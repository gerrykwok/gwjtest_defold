local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test28 = gwjui.class("test28")

function test28:ctor()
end

function test28:onEnter()
	gwjui.ScaleButton.new({
		main_id = "btn_vibrate",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickVibrate()
	end)
end

function test28:onExit()
end

function test28:on_message(message_id, message, sender)
end

function test28:onClickVibrate()
	if(misc) then
		misc.vibrate(2)
	end
end

return test28
