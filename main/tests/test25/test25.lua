local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test25 = gwjui.class("test25")

function test25:ctor()
end

function test25:onEnter()
	self.m_url = msg.url()

	self:showBattery()
	timer.delay(1, true, function()
		self:showBattery()
	end)
end

function test25:onExit()
end

function test25:on_message(message_id, message, sender)
end

function test25:showBattery()
	local level,charging = misc.getBatteryInfo()
	local text = string.format("level:%d,plugged:%s", level, tostring(charging))
	local node = gui.get_node("text_batteryinfo")
	gui.set_text(node, text)
end

return test25
