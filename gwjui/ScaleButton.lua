--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-16
-- Description: 按下时会放大的按钮

local gwjui = require("gwjui.gwjui_impl")
local UIPushButton = require("gwjui.UIPushButton")

local clsScaleButton = gwjui.class("ScaleButton", UIPushButton)

function clsScaleButton:ctor(params)
	clsScaleButton.super.ctor(self, params)
	self.m_maxScale = params.maxScale or 1
	self.m_scaleTime = params.scaleTime or 0.15
	self.m_onPressed = nil
	self.m_onReleased = nil
	self.m_onClicked = nil
	clsScaleButton.super.onButtonPressed(self, function()
		self:scaleTo(self.m_maxScale)
		gwjui.callfunc(self.m_onPressed)
	end)
	clsScaleButton.super.onButtonReleased(self, function()
		self:scaleTo(1)
		gwjui.callfunc(self.m_onReleased)
	end)
	clsScaleButton.super.onButtonClicked(self, function()
		self:scaleTo(1)
		gwjui.callfunc(self.m_onClicked)
	end)
end

function clsScaleButton:onButtonPressed(func)
	self.m_onPressed = func
	return self
end

function clsScaleButton:onButtonReleased(func)
	self.m_onReleased = func
	return self
end

function clsScaleButton:onButtonClicked(func)
	self.m_onClicked = func
	return self
end

function clsScaleButton:scaleTo(scale)
	gui.animate(self:getMainNode(), "scale", vmath.vector3(scale, scale, 1), gui.EASING_LINEAR, self.m_scaleTime)
end

return clsScaleButton
