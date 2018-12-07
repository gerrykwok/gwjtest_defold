local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

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
		self:setLoginInfo(message)
	end
end

function test14:onClickWechatLogin()
	gwjui.printf("login wechat")
	wechat.login(function(script, res)
		self:onWechatLoginResult(res)
	end)
end

function test14:onWechatLoginResult(res)
	gwjui.printf("wechat login res=%s", tostring(res))
	local t = json.decode(res)
	gwjui.dump(t, "gwjgwj,login result", 100)
	self:setLoginInfo(t)
--	msg.post(self.m_url, "wechat_login_res", t)
end

function test14:setLoginInfo(t)
	local errCode = t.errCode
	if(errCode == 0) then
		gui.set_text(gui.get_node("text_wechat_login_code"), t.code or "")
		gui.set_text(gui.get_node("text_wechat_login_lang"), t.lang or "")
		gui.set_text(gui.get_node("text_wechat_login_country"), t.country or "")
	elseif(errCode == -2) then
		TipsBanner.show("用户取消微信登录")
	else
		local str = string.format("微信登录失败,errCode=%d,errStr=%s", errCode, t.errStr)
		TipsBanner.show(str)
	end
end

function test14:onClickWechatLogout()
	gwjui.printf("logout wechat")
	wechat.logout()
end

return test14
