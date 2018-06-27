--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-19
-- Description: 用于接收用户输入的对象

local gwjui = require("gwjui.gwjui_impl")
local gwjinput = require("gwjui.gwjinput")

local clsInputObject = gwjui.class("InputObject")

function clsInputObject:ctor(params)
	params = params or {}
	local main_id = params.main_id
	local main_node = params.main_node
	if(main_node) then
		self.m_mainNode = main_node
	else
		self.m_mainNode = gui.get_node(main_id)
	end
	self.m_touchEnabled = false
	self.m_touchSwallowEnabled = true
	self.m_name = ""
	self.m_url = tostring(msg.url())

	local input = gwjinput.getCurrentInput()
	if(input) then
		input:addObject(self)
	end
end

function clsInputObject:onExit()
	local input = gwjinput.getCurrentInput()
	if(input) then
		input:removeObject(self)
	end
end

function clsInputObject:getMainNode()
	return self.m_mainNode
end

function clsInputObject:setName(name)
	self.m_name = name
end

function clsInputObject:getName()
	return self.m_name
end

function clsInputObject:setTouchEventListener(func)
	self.m_onTouchEvent = func
	return self
end

function clsInputObject:onTouchEvent(event)
	if(self.m_onTouchEvent) then
		self.m_onTouchEvent(event)
	end
end

function clsInputObject:setTextEventListener(func)
	self.m_onText = func
	return self
end

function clsInputObject:onTextEvent(event)
	if(tostring(msg.url()) ~= self.m_url) then
		return
	end
	if(self.m_onText) then
		self.m_onText(event)
	end
end

function clsInputObject:setBackspaceListener(func)
	self.m_onBackspace = func
	return self
end

function clsInputObject:onBackspace(event)
	if(tostring(msg.url()) ~= self.m_url) then
		return
	end
	if(self.m_onBackspace) then
		self.m_onBackspace(event)
	end
end

function clsInputObject:setTouchEnabled(enable)
	self.m_touchEnabled = enable
end

function clsInputObject:isTouchEnabled()
	return self.m_touchEnabled
end

function clsInputObject:setTouchSwallowEnabled(enable)
	self.m_touchSwallowEnabled = enable
end

function clsInputObject:isTouchSwallowEnabled()
	return self.m_touchSwallowEnabled
end

function clsInputObject:setVisible(visible)
	gui.set_enabled(self.m_mainNode, visible)
end

function clsInputObject:isVisible()
	return gui.is_enabled(self.m_mainNode) -- and gui.get_color(self.m_mainNode).w > 0
	--[[	
	local flag,enable = pcall(function(node)
		return gui.is_enabled(node)
	end, self.m_mainNode)
	if(not flag) then enable = false end
	if(not enable) then return false end
	local color = gui.get_color(self.m_mainNode)
	return color.w > 0
	]]
end

function clsInputObject:isVisibleInScreen()
	if(not self:isVisible()) then return false end
	local node = gui.get_parent(self.m_mainNode)
	while(node) do
		if(not gui.is_enabled(node)) then
			return false
		end
		node = gui.get_parent(node)
	end
	return true
end

function clsInputObject:onReleaseKBCapture()
end

function clsInputObject:getUrl()
	return self.m_url
end

return clsInputObject
