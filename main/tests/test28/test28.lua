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
	gwjui.ScaleButton.new({
		main_id = "btn_vibrate1",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickVibrate1()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_vibrate2",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickVibrate2()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_vibrate3",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickVibrate3()
	end)
end

function test28:onExit()
end

function test28:on_message(message_id, message, sender)
end

function test28:onClickVibrate()
	self:doVibrate(2.5)
end

function test28:onClickVibrate1()
	self:doVibrate(10000)
end

function test28:onClickVibrate2()
	self:doVibrate(20000)
end

function test28:onClickVibrate3()
	self:doVibrate(30000)
end

function test28:doVibrate(timeInSecond)
	if(misc) then
		misc.vibrate(timeInSecond)
	end
end

return test28
