--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-19
-- Description: 按下时会变暗的按钮

local gwjui = require("gwjui.gwjui")
local GwjUIPushButton = require("gwjui.GwjUIPushButton")

local clsGwjDarkenButton = gwjui.class("GwjDarkenButton", GwjUIPushButton)

function clsGwjDarkenButton:ctor(params)
	clsGwjDarkenButton.super.ctor(self, params)

	self.m_colorClicked = params.colorClicked or vmath.vector4(0.75, 0.75, 0.75, 1)

	self.m_onPressed = nil
	self.m_onReleased = nil
	self.m_onClicked = nil
	clsGwjDarkenButton.super.onButtonPressed(self, function()
		self:setClicked_(true)
		gwjui.callfunc(self.m_onPressed)
	end)
	clsGwjDarkenButton.super.onButtonReleased(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onReleased)
	end)
	clsGwjDarkenButton.super.onButtonClicked(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onClicked)
	end)
end

function clsGwjDarkenButton:onButtonPressed(func)
	self.m_onPressed = func
	return self
end

function clsGwjDarkenButton:onButtonReleased(func)
	self.m_onReleased = func
	return self
end

function clsGwjDarkenButton:onButtonClicked(func)
	self.m_onClicked = func
	return self
end

function clsGwjDarkenButton:setClicked_(clicked)
	local color = clicked and self.m_colorClicked or vmath.vector4(1, 1, 1, 1)
	gui.set_color(self.m_mainNode, color)
end

return clsGwjDarkenButton
