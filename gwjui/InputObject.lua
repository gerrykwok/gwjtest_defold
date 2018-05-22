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
	self.m_mainNode = gui.get_node(main_id)
end

function clsInputObject:getMainNode()
	return self.m_mainNode
end

function clsInputObject:setTouchEventListener(func)
	if(gwjinput.s_recentlyInstance) then
		gwjinput.s_recentlyInstance:addObject(self)
	end
	self.m_onTouchEvent = func
	return self
end

function clsInputObject:onTouchEvent(event)
	if(self.m_onTouchEvent) then
		self.m_onTouchEvent(event)
	end
end

function clsInputObject:setVisible(visible)
	gui.set_enabled(self.m_mainNode, visible)
end

function clsInputObject:isVisible()
	local color = gui.get_color(self.m_mainNode)
	return gui.is_enabled(self.m_mainNode) and color.w > 0
end

return clsInputObject
