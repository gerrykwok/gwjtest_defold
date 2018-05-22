--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-19
-- Description: 按下时会变暗的按钮

local gwjui = require("gwjui.gwjui_impl")
local UIPushButton = require("gwjui.UIPushButton")

local clsDarkenButton = gwjui.class("DarkenButton", UIPushButton)

function clsDarkenButton:ctor(params)
	clsDarkenButton.super.ctor(self, params)

	self.m_colorClicked = params.colorClicked or vmath.vector4(0.75, 0.75, 0.75, 1)

	self.m_onPressed = nil
	self.m_onReleased = nil
	self.m_onClicked = nil
	clsDarkenButton.super.onButtonPressed(self, function()
		self:setClicked_(true)
		gwjui.callfunc(self.m_onPressed)
	end)
	clsDarkenButton.super.onButtonReleased(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onReleased)
	end)
	clsDarkenButton.super.onButtonClicked(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onClicked)
	end)
end

function clsDarkenButton:onButtonPressed(func)
	self.m_onPressed = func
	return self
end

function clsDarkenButton:onButtonReleased(func)
	self.m_onReleased = func
	return self
end

function clsDarkenButton:onButtonClicked(func)
	self.m_onClicked = func
	return self
end

function clsDarkenButton:setClicked_(clicked)
	local color = clicked and self.m_colorClicked or vmath.vector4(1, 1, 1, 1)
	gui.set_color(self.m_mainNode, color)
end

return clsDarkenButton
