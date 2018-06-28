require("main.common.MiscUtil")
local gwjui = require("gwjui.gwjui")

local TextureCache = require("gwjui.TextureCache")
local luaj = require("main.tests.test12.plmext.luaj")

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
	:onButtonClicked(function()
		gwjui.printf("upload head,path=%s", self.m_localPath)
	end)
end

function test12:onExit()
end

function test12:on_message(message_id, message, sender)
	if(message_id == hash("get_photo_result")) then
		local localPath = message.path
		local res = tonumber(message.res)
		gwjui.dump(message, "get photo result")
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
end

function test12:onGetPhoto(fromCamera)
	gwjui.printf("LuaJavaBridge=%s(%s)", tostring(LuaJavaBridge), type(LuaJavaBridge))

	local imageWidth = 320
	local imageHeight = 320
	local localPath = sys.get_save_file("plm", "avatarout.png")
	--localPath = "/storage/emulated/0/avatarout.png"
	local source = fromCamera and "camera" or "gallery"
	gwjui.printf("gwjgwj,get from %s,localPath=%s", source, localPath)
	if(device.platform == "android") then
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
	else
--		plmext.getPhoto(fromCamera, localPath, imageWidth, imageHeight, function(res)
--			gwjui.printf("gwjgwj,get photo res=%s(%s)", tostring(res), type(res))
--			msg.post(self.m_url, "get_photo_result", {res=res, path=localPath})
--		end)
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

return test12
