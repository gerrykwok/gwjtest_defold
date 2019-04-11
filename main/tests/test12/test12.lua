require("main.common.MiscUtil")
local gwjui = require("gwjui.gwjui")

local TextureCache = require("gwjui.TextureCache")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")
local LoadingView = require("main.common.loading.LoadingView")

local device = {}
local info = sys.get_sys_info()
local sysName = info.system_name
if(sysName == "Android") then
	device.platform = "android"
elseif(sysName == "iPhone OS") then
	device.platform = "ios"
elseif(sysName == "Windows") then
	device.platform = "windows"
elseif(sysName == "Darwin") then
	device.platform = "mac"
end

local test12 = gwjui.class("test12")

function test12:ctor()
end

function test12:onEnter()
	self.m_url = msg.url()
	self.m_updateCount = 0
	self.m_localPath = ""

	--从相册选择图片
	local btn = gwjui.ScaleButton.new({
		main_id = "btn_get_local_picture",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onGetPhoto(false)
	end)
	--摄像机获取图片
	local btn = gwjui.ScaleButton.new({
		main_id = "btn_get_camera_picture",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onGetPhoto(true)
	end)
	--userid
	local input = gwjui.UIInput.new({
		main_id = "input_userid",
		text = "0",
		textColor = vmath.vector4(1, 0, 0, 1),
		textFont = "test_24",
		placeholder = "输入userId",
		placeholderColor = vmath.vector3(0.5, 0.5, 0.5),
		listener = function(event, edit)
			gwjui.printf("edit event:%s text=%s", tostring(event), edit:getText())
		end,
	})
	self.m_inputUserId = input
	--上传头像按钮
	local btn = gwjui.ScaleButton.new({
		main_id = "btn_upload_head",
		maxScale = 1.1,
	})
	:onButtonClicked(gwjui.handler(self, self.onClickUpload))
	--测试按钮
	local btn = gwjui.ScaleButton.new({
		main_id = "btn_test",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		testext.test()
	end)
	--delay test
	gwjui.ScaleButton.new({
		main_id = "btn_test_delay",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		testext.test_delay(2.5)
	end)
end

function test12:onExit()
end

function test12:on_message(message_id, message, sender)
end

function test12:onGetPhoto(fromCamera)
	local imageWidth = 320
	local imageHeight = 320
	gwjui.printf("gwjgwj,onGetPhoto")
	local localPath = sys.get_save_file(sys.get_config("project.title"), "avatarout.jpg")
	--localPath = "/storage/emulated/0/avatarout.png"
	gwjui.printf("gwjgwj,get from %s,localPath=%s", fromCamera and "camera" or "gallery", localPath)
	if(takephoto == nil) then return end
	local ok,res = takephoto.getPhoto({
		fromCamera = fromCamera,
		localPath = localPath,
		imageWidth = imageWidth,
		imageHeight = imageHeight,
		callback = function(script, res)
			gwjui.printf("gwjgwj,callback res = %s", tostring(res))
			local t = json.decode(res)

			if(t.result == 1) then
				TextureCache.getInstance():removeTextureForKey(localPath)
				local count = 0
				local function func()
					count = count + 1
					if(count >= 4) then
						gwjui.unscheduleUpdate(func)
						self:onGetPhotoResult(t.result, localPath)
					end
				end
				gwjui.scheduleUpdate(func)
			else
				TipsBanner.show("取消")
			end
		end,
	})
	gwjui.printf("takephoto,ok=%s,res=%s", tostring(ok), tostring(res))
end

function test12:onGetPhotoResult(res, path)
	if(tostring(res) == "1") then
		local textureId = TextureCache.getInstance():addImage(path)
		if(textureId) then
			local node = gui.get_node("head")
			gui.set_texture(node, textureId)
			self.m_localPath = path
		end
	end
end

function test12:onClickUpload()
	--牌乐门所用的token
	--local qiniu_token = "oYOp_cUtKDHNJv9fQ1e5eYuo8FAce3q_RakmZC-A:Jd83YHh2JhLLkmJR0Ivle5F4uSo=:eyJzY29wZSI6ImFuZGVnYW1lLXRlc3QiLCJkZWFkbGluZSI6MTU1Mzk4MTQ5MX0="
	--伟基测试的token和域名
	local url_prefix = "http://pb2pekr3y.bkt.clouddn.com"
	local qiniu_token = "34kE2v75Sx0LwsTZRZz8fDEUqpolcgYWL87f9FRo:zB39WK-Wi29Bm9FfvTC2lMazYkg=:eyJzY29wZSI6Imd3anRlc3QiLCJkZWFkbGluZSI6MTU3NzgwODAwMH0="
	gwjui.printf("upload head,path=%s", self.m_localPath)
	local localPath = self.m_localPath or ""
--	local key = string.format("gwjtest/test_%d.jpg", os.time())
	local key = string.format("gwjtest/test_qiniu_%s.jpg", self.m_inputUserId:getText())
	gwjui.printf("key=%s", key)
	if(localPath == "") then
		TipsBanner.show("请先选择图片")
		return
	end
	if(qiniu == nil) then return end
	self:setLoadingView(true)
	local ok,res = qiniu.uploadFile({
		token = qiniu_token,
		path = localPath,
		key = key,
		callback = function(script, res)
			self:setLoadingView(false)
			--TipsBanner.show("res=" .. tostring(res))
			local t = json.decode(res)
			if(t.result == 0) then
				TipsBanner.show("上传成功")
			elseif(t.result == 1) then
				TipsBanner.show("目标资源已存在")
			else
				TipsBanner.show("上传失败:" .. t.errMsg)
			end
		end,
	})
	gwjui.printf("uploadfile,ok=%s,res=%s", tostring(ok), tostring(res))
--	if(device.platform == "android") then
--		self:setLoadingView(true)
--		local javaClassName = "com/xishanju/plm/plmext/UploadHead"
--		local javaMethodName = "uploadHead"
--		local javaParams = {
--			qiniu_token,
--			localPath,
--			key,
--			function(res)
--				gwjui.printf("gwjgwj,res=%s", tostring(res))
--				msg.post(self.m_url, "upload_result", {res=res})
--			end
--		}
--		--String token, String path, String name, final int luaCallbackFunction
--		local javaMethodSig = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"
--		luaj.callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethodSig)
--	elseif(device.platform == "ios") then
--		self:setLoadingView(true)
--		local args = {
--			token = qiniu_token,
--			filepath = localPath,
--			key = key,
--			callback = function(res)
--				gwjui.printf("gwjgwj,ios upload photo res:"..res)
--				local data = json.decode(res)
--				if(data and data.result == 0) then
--					res = "1"
--				else
--					res = "0"
--				end
--				msg.post(self.m_url, "upload_result", {res=res})
--			end
--		}
--		luaoc.callStaticMethod("UploadImageBridge", "uploadImage", args)
--	end
end

function test12:setLoadingView(flag)
	if(flag) then
		if(self.m_loading == nil) then
			self.m_loading = LoadingView.show()
		end
	else
		if(self.m_loading) then
			self.m_loading:close()
			self.m_loading = nil
		end
	end
end

return test12
