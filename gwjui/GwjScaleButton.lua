--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-16
-- Description: 按下时会放大的按钮

local gwjui = require("gwjui.gwjui")
local GwjUIPushButton = require("gwjui.GwjUIPushButton")

local clsGwjScaleButton = gwjui.class("", GwjUIPushButton)

function clsGwjScaleButton.createInstance(...)
	return clsGwjScaleButton.new(...)
end

function clsGwjScaleButton:ctor(params)
	clsGwjScaleButton.super.ctor(self, params)
	self.m_maxScale = params.maxScale or 1
	self.m_scaleTime = params.scaleTime or 0.15
	self.m_onPressed = nil
	self.m_onReleased = nil
	self.m_onClicked = nil
	clsGwjScaleButton.super.onButtonPressed(self, function()
		self:scaleTo(self.m_maxScale)
		gwjui.callfunc(self.m_onPressed)
	end)
	clsGwjScaleButton.super.onButtonReleased(self, function()
		self:scaleTo(1)
		gwjui.callfunc(self.m_onReleased)
	end)
	clsGwjScaleButton.super.onButtonClicked(self, function()
		self:scaleTo(1)
		gwjui.callfunc(self.m_onClicked)
	end)
end

function clsGwjScaleButton:onButtonPressed(func)
	self.m_onPressed = func
	return self
end

function clsGwjScaleButton:onButtonReleased(func)
	self.m_onReleased = func
	return self
end

function clsGwjScaleButton:onButtonClicked(func)
	self.m_onClicked = func
	return self
end

function clsGwjScaleButton:scaleTo(scale)
	gui.animate(self:getMainNode(), "scale", vmath.vector3(scale, scale, 1), gui.EASING_LINEAR, self.m_scaleTime)
end

return clsGwjScaleButton
