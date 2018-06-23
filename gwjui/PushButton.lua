--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-18
-- Description: 按下时会换图的按钮

local gwjui = require("gwjui.gwjui_impl")
local UIPushButton = require("gwjui.UIPushButton")

local clsPushButton = gwjui.class("PushButton", UIPushButton)

function clsPushButton:ctor(params)
	clsPushButton.super.ctor(self, params)

	local normal_id = params.normal_id
	local clicked_id = params.clicked_id
	local normal_node = params.normal_node
	local clicked_node = params.clicked_node
	if(normal_node) then self.m_nodeNormal = normal_node
	elseif(normal_id) then self.m_nodeNormal = gui.get_node(normal_id)
	end
	if(clicked_node) then self.m_nodeClicked = clicked_node
	elseif(clicked_id) then self.m_nodeClicked = gui.get_node(clicked_id)
	end

	self.m_texNormal = params.texNormal
	self.m_texClicked = params.texClicked
	
	self:setClicked_(false)

	self.m_onPressed = nil
	self.m_onReleased = nil
	self.m_onClicked = nil
	clsPushButton.super.onButtonPressed(self, function()
		self:setClicked_(true)
		gwjui.callfunc(self.m_onPressed)
	end)
	clsPushButton.super.onButtonReleased(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onReleased)
	end)
	clsPushButton.super.onButtonClicked(self, function()
		self:setClicked_(false)
		gwjui.callfunc(self.m_onClicked)
	end)
end

function clsPushButton:onButtonPressed(func)
	self.m_onPressed = func
	return self
end

function clsPushButton:onButtonReleased(func)
	self.m_onReleased = func
	return self
end

function clsPushButton:onButtonClicked(func)
	self.m_onClicked = func
	return self
end

function clsPushButton:setClicked_(clicked)
	if(self.m_texNormal and self.m_texClicked) then
		gui.play_flipbook(self.m_mainNode, clicked and self.m_texClicked or self.m_texNormal)
	else
		gui.set_enabled(self.m_nodeNormal, not clicked)
		gui.set_enabled(self.m_nodeClicked, clicked)
	end
end

return clsPushButton
