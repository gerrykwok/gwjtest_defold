local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test19 = gwjui.class("test19")

function test19:ctor()
end

function test19:onEnter()
	self.m_url = msg.url()

	gwjui.ScaleButton.new({
		main_id = "btn_xianliao_login",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onXianliaoLogin()
	end)

	gwjui.ScaleButton.new({
		main_id = "btn_xianliao_share_text",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onXianliaoShare_text()
	end)

	gwjui.ScaleButton.new({
		main_id = "btn_xianliao_share_image",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onXianliaoShare_image()
	end)

	gwjui.ScaleButton.new({
		main_id = "btn_xianliao_share_app",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onXianliaoShare_app()
	end)
end

function test19:onExit()
end

function test19:on_message(message_id, message, sender)
end

function test19:onXianliaoLogin()
	local ok,ret = xianliao.login({
		callback = function(script, res)
			self:onLoginResult(res)
		end,
	})
	gwjui.printf("gwjgwj,xianliao login,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

function test19:onLoginResult(res)
	gwjui.printf("xianliao login res=%s", tostring(res))
	local t = json.decode(res)
	gwjui.dump(t, "gwjgwj,login result", 100)
	self:setLoginInfo(t)
end

function test19:setLoginInfo(t)
	local errCode = t.errCode
	if(errCode == 0) then
		gui.set_text(gui.get_node("txt_code"), "code:" .. (t.code or ""))
		gui.set_text(gui.get_node("txt_lang"), "lang:" .. (t.lang or ""))
		gui.set_text(gui.get_node("txt_country"), "country:" .. (t.country or ""))
	elseif(errCode == -2) then
		TipsBanner.show("用户取消登录")
	else
		local str = string.format("登录失败,errCode=%d,errStr=%s", errCode, t.errStr)
		TipsBanner.show(str)
	end
end

function test19:onXianliaoShare_text()
	local ok,ret = xianliao.share({
		type = "text",
		text = "要分享的文字",
		callback = function(script, res)
			self:onShareResult(res)
		end,
	})
	gwjui.printf("gwjgwj,xianliao share text,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

function test19:onXianliaoShare_image()
	local ok,ret = xianliao.share({
		type = "image",
		image = "IMG_20181204_094608.jpg",
		imageFrom = 2,
		callback = function(script, res)
			self:onShareResult(res)
		end,
	})
	gwjui.printf("gwjgwj,xianliao share image,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

function test19:onXianliaoShare_app()
	local ok,ret = xianliao.share({
		type = "app",
		image = "IMG_20181205_203508.jpg",
		imageFrom = 2,
		title = "app title",
		text = "app text",
		url = "http://www.pconline.com.cn",
		callback = function(script, res)
			self:onShareResult(res)
		end,
	})
	gwjui.printf("gwjgwj,xianliao share app,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

function test19:onShareResult(res)
	gwjui.printf("gwjgwj,share result:%s", tostring(res))
	TipsBanner.show("分享结果:" .. tostring(res))
end

return test19
