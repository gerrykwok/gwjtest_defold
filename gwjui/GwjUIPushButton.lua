--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-16
-- Description: PushButton

local gwjui = require("gwjui.gwjui")
local GwjInputObject = require("gwjui.GwjInputObject")

local clsGwjUIPushButton = gwjui.class("GwjUIPushButton", GwjInputObject)
if false then
	GwjUIPushButton = clsGwjUIPushButton
end

function clsGwjUIPushButton:ctor(params)
	clsGwjUIPushButton.super.ctor(self, params)

	self.m_onButtonPressed = nil
	self.m_onButtonReleased = nil
	self.m_onButtonClicked = nil
	self.m_pressed = false
end

function clsGwjUIPushButton:onTouch_(event)
--	print("GwjUIPushButton:onTouch_:", event.name)
	if(event.name == "began") then
		self.m_pressed = true
		gwjui.callfunc(self.m_onButtonPressed)
	elseif(event.name == "moved") then
		local pressed = self:hitTest(event.x, event.y)
		if(pressed ~= self.m_pressed) then
			self.m_pressed = pressed
			gwjui.callfunc(pressed and self.m_onButtonPressed or self.m_onButtonReleased)
		end
	elseif(event.name == "ended") then
		if(self:hitTest(event.x, event.y)) then
			gwjui.callfunc(self.m_onButtonClicked)
		end
	end
end

function clsGwjUIPushButton:onButtonPressed(func)
	self.m_onButtonPressed = func
	return self
end

function clsGwjUIPushButton:onButtonReleased(func)
	self.m_onButtonReleased = func
	return self
end

function clsGwjUIPushButton:onButtonClicked(func)
	self.m_onButtonClicked = func
	return self
end

--判断坐标是否落在按钮上
function clsGwjUIPushButton:hitTest(x, y)
	return gui.pick_node(self.m_mainNode, x, y)
end

return clsGwjUIPushButton
