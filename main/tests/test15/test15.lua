local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test15 = gwjui.class("test15")

function test15:ctor()
end

function test15:onEnter()
	self.m_url = msg.url()
	gwjui.ScaleButton.new({
		main_id = "btn_share_system",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareSystem()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_share_wechat_sdk",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareWechatSDK()
	end)
end

function test15:onExit()
end

function test15:on_message(message_id, message, sender)
end

function test15:onClickShareSystem()
	gwjui.printf("gwjgwj,share system")
end

function test15:onClickShareWechatSDK()
end

return test15
