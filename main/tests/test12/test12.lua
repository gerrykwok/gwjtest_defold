require("main.common.MiscUtil")
local gwjui = require("gwjui.gwjui")

local TextureCache = require("gwjui.TextureCache")
local luaj = require("cocosext.luaj")
local luaoc = require("cocosext.luaoc")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local device = {}
local info = sys.get_sys_info()
local sysName = info.system_name
--ccprint("gwjgwj,system_name=%s", sysName)
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
	--上传头像按钮
	local btn = gwjui.ScaleButton.new({
		main_id = "btn_upload_head",
		maxScale = 1.1,
	})
	:onButtonClicked(gwjui.handler(self, self.onClickUpload))
end

function test12:onExit()
end

function test12:on_message(message_id, message, sender)
	if(message_id == hash("get_photo_result")) then
		local localPath = message.path
		local res = tonumber(message.res)
		gwjui.dump(message, "get photo result")
		if(tostring(res) == "1") then
			TextureCache.getInstance():removeTextureForKey(localPath)
			local count = 0
			local function func()
				count = count + 1
				if(count >= 4) then
					gwjui.unscheduleUpdate(func)
					self:onGetPhotoResult(res, localPath)
				end
			end
			gwjui.scheduleUpdate(func)
		end
	elseif(message_id == hash("upload_result")) then
		local str = string.format("上传结果:%s", tostring(message.res))
		TipsBanner.show(str)
	end
end

function test12:onGetPhoto(fromCamera)
	local imageWidth = 320
	local imageHeight = 320
	gwjui.printf("gwjgwj,onGetPhoto")
	local localPath = sys.get_save_file("plm", "avatarout.jpg")
	--localPath = "/storage/emulated/0/avatarout.png"
	local source = fromCamera and "camera" or "gallery"
	gwjui.printf("gwjgwj,get from %s,localPath=%s", source, localPath)
	if(device.platform == "android") then
		gwjui.printf("LuaJavaBridge=%s(%s)", tostring(LuaJavaBridge), type(LuaJavaBridge))
		--getPhoto(Context ctx, int fromCamera, String localPath, int width, int height, int luaCallbackFunction)
		local javaClassName = "com/xishanju/plm/plmext/plmext"
		local javaMethodName = "getPhoto"
		local javaParams = {
			fromCamera and 1 or 0,
			localPath,
			imageWidth,
			imageHeight,
			function(res)
				gwjui.printf("gwjgwj,getPhoto res=%s", tostring(res))
				msg.post(self.m_url, "get_photo_result", {res=res, path=localPath})
			end
		}
		local javaMethodSig = "(ILjava/lang/String;III)V"
		luaj.callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethodSig)
	elseif(device.platform == "ios") then
		gwjui.printf("LuaObjcBridge=%s(%s)", tostring(LuaObjcBridge), type(LuaObjcBridge))
		local args = {
			fromCamera = fromCamera and 1 or 0,--是否从摄像头获取
			path = localPath,
			width = imageWidth,
			height = imageHeight,
			callback = function(res)
				msg.post(self.m_url, "get_photo_result", {res=res, path=localPath})
			end
		}
		luaoc.callStaticMethod("TakePhoto", "takePicture", args)
	end
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
	local qiniu_token = "34kE2v75Sx0LwsTZRZz8fDEUqpolcgYWL87f9FRo:zB39WK-Wi29Bm9FfvTC2lMazYkg=:eyJzY29wZSI6Imd3anRlc3QiLCJkZWFkbGluZSI6MTU3NzgwODAwMH0="
	gwjui.printf("upload head,path=%s", self.m_localPath)
	local localPath = self.m_localPath or ""
	local key = string.format("gwjtest/test_%d.jpg", os.time())
	if(localPath == "") then return end
	if(device.platform == "android") then
		local javaClassName = "com/xishanju/plm/plmext/UploadHead"
		local javaMethodName = "uploadHead"
		local javaParams = {
			qiniu_token,
			localPath,
			key,
			function(res)
				gwjui.printf("gwjgwj,res=%s", tostring(res))
				msg.post(self.m_url, "upload_result", {res=res})
			end
		}
		--String token, String path, String name, final int luaCallbackFunction
		local javaMethodSig = "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;I)V"
		luaj.callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethodSig)
	elseif(device.platform == "ios") then
		local args = {
			token = qiniu_token,
			filepath = localPath,
			key = key,
			callback = function(res)
				gwjui.printf("gwjgwj,ios upload photo res:"..res)
				--[[
				local success=false
				if res then
					local resJson=json.decode(res)
					if resJson and resJson.result==0 then
						success=true
					end
				end
				if success then
					DBManager.updateImage(ImageManager.getImageIdForAvatar(User.userId), self.logoFileName)
					-- self:_saveHeadData(self.defaultHeadList:getCurLogoId(), self.logoUrl)
					--已经上传成功，接着保存到数据库
					self.logoLocalPath=nil
					self:save()
				else
					TipsBanner.show("上传头像失败")
					self:setLoadingView(false)
					self.uploadlHeadFail=true
				end
				]]
			end
		}
		luaoc.callStaticMethod("AppController", "uploadHead", args)
	end
end

return test12
