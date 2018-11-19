local gwjui = require("gwjui.gwjui")
local luaj = require("extension.cocosext.luaj")
local luaoc = require("extension.cocosext.luaoc")

local test14 = gwjui.class("test14")

function test14:ctor()
end

function test14:onEnter()
	self.m_url = msg.url()
	local btnLoginWechat = gwjui.ScaleButton.new({
		main_id = "btn_login_wechat",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickWechatLogin()
	end)
	local btnLogoutWechat = gwjui.ScaleButton.new({
		main_id = "btn_logout_wechat",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickWechatLogout()
	end)
end

function test14:onExit()
end

function test14:on_message(message_id, message, sender)
	if(message_id == hash("wechat_login_res")) then
		gui.set_text(gui.get_node("text_wechat_nickname"), message.userInfo.nickname);
		gui.set_text(gui.get_node("text_wechat_icon_url"), message.userInfo.profileImage);
	end
end

function test14:onClickWechatLogin()
	gwjui.printf("login wechat")
	local sysName = sys.get_sys_info().system_name
	--ccprint("gwjgwj,system_name=%s", sysName)
	if(sysName == "Android") then
		local javaClassName = "com/xishanju/plm/plmext/PlatformWechat"
		local javaMethodName = "login"
		local javaParams = {
			4,
			function(res)
				self:onWechatLoginResult(res)
			end,
		}
		local javaMethodSig = "(II)V"
		local ok, res = luaj.callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethodSig)
	elseif(sysName == "iPhone OS") then
		local args = {
			callback = function(res)
				self:onWechatLoginResult(res)
			end,
		}
		luaoc.callStaticMethod("PlatformWechat", "login", args)
	elseif(sysName == "Windows") then
	elseif(sysName == "Darwin") then
	end
end

function test14:onWechatLoginResult(res)
	gwjui.printf("wechat login res=%s", tostring(res))
	local t = json.decode(res)
	gwjui.dump(t, "tttttt", 100)
--	gui.set_text(gui.get_node("text_wechat_nickname"), t.userInfo.nickname);
--	gui.set_text(gui.get_node("text_wechat_icon_url"), t.userInfo.profileImage);
	msg.post(self.m_url, "wechat_login_res", t)
end

function test14:onClickWechatLogout()
	gwjui.printf("logout wechat")
	local sysName = sys.get_sys_info().system_name
	--ccprint("gwjgwj,system_name=%s", sysName)
	if(sysName == "Android") then
		local javaClassName = "com/xishanju/plm/plmext/PlatformWechat"
		local javaMethodName = "logout"
		local javaParams = {}
		local javaMethodSig = "()V"
		local ok, res = luaj.callStaticMethod(javaClassName, javaMethodName, javaParams, javaMethodSig)
	elseif(sysName == "iPhone OS") then
	elseif(sysName == "Windows") then
	elseif(sysName == "Darwin") then
	end
end

return test14
