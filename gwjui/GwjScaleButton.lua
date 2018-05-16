--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-16
-- Description: 缩放的按钮

local gwjui = require("gwjui.gwjui")
local GwjUIPushButton = require("gwjui.GwjUIPushButton")

local clsGwjScaleButton = gwjui.class("", GwjUIPushButton)

function clsGwjScaleButton.createInstance(...)
	return clsGwjScaleButton.new(...)
end

function clsGwjScaleButton:ctor(params)
	clsGwjScaleButton.super.ctor(self, params)
end

function clsGwjScaleButton:onButtonPressed(func)
	return clsGwjScaleButton.super.onButtonPressed(self, function()
		func()
		gui.animate(self:getMainNode(), "scale", vmath.vector3(1.1, 1.1, 1), gui.EASING_LINEAR, 0.2)
	end)
end

function clsGwjScaleButton:onButtonReleased(func)
	return clsGwjScaleButton.super.onButtonReleased(self, function()
		func()
		gui.animate(self:getMainNode(), "scale", vmath.vector3(1, 1, 1), gui.EASING_LINEAR, 0.2)
	end)
end

function clsGwjScaleButton:onButtonClicked(func)
	return clsGwjScaleButton.super.onButtonClicked(self, function()
		func()
		gui.animate(self:getMainNode(), "scale", vmath.vector3(1, 1, 1), gui.EASING_LINEAR, 0.2)
	end)
end

return clsGwjScaleButton
