--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-18
-- Description: 按下时会换图的按钮

local gwjui = require("gwjui.gwjui")
local GwjUIPushButton = require("gwjui.GwjUIPushButton")

local clsGwjPushButton = gwjui.class("GwjPushButton", GwjUIPushButton)

function clsGwjPushButton.createInstance(...)
	return clsGwjPushButton.new(...)
end

function clsGwjPushButton:ctor(params)
	clsGwjPushButton.super.ctor(self, params)

	local normal_id = params.normal_id
	local clicked_id = params.clicked_id
	self.m_nodeNormal = gui.get_node(normal_id)
	self.m_nodeClicked = gui.get_node(clicked_id)
	self:setClicked_(false)

	self.m_onPressed = nil
	self.m_onReleased = nil
	self.m_onClicked = nil
	clsGwjPushButton.super.onButtonPressed(self, function()
		self:setClicked_(true)
		gwjui.callfunc(self.m_onPressed)
	end)
	clsGwjPushButton.super.onButtonReleased(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onReleased)
	end)
	clsGwjPushButton.super.onButtonClicked(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onClicked)
	end)
end

function clsGwjPushButton:onButtonPressed(func)
	self.m_onPressed = func
	return self
end

function clsGwjPushButton:onButtonReleased(func)
	self.m_onReleased = func
	return self
end

function clsGwjPushButton:onButtonClicked(func)
	self.m_onClicked = func
	return self
end

function clsGwjPushButton:setClicked_(clicked)
	gui.set_enabled(self.m_nodeNormal, not clicked)
	gui.set_enabled(self.m_nodeClicked, clicked)
end

return clsGwjPushButton
