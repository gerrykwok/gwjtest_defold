local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test22 = gwjui.class("test22")

function test22:ctor()
end

function test22:onEnter()
	self.m_url = msg.url()

	gwjui.ScaleButton.new({
		main_id = "btn_pay_test",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickPayTest()
	end)
end

function test22:onExit()
end

function test22:on_message(message_id, message, sender)
end

function test22:onClickPayTest()
	testext.printMacro()
end

return test22
