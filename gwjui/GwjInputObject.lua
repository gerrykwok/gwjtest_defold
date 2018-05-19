--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-19
-- Description: 用于接收用户输入的对象

local gwjui = require("gwjui.gwjui")
local gwjinput = require("gwjui.gwjinput")

local clsGwjInputObject = gwjui.class("GwjInputObject")

function clsGwjInputObject:ctor(params)
	if(gwjinput.s_recentlyInstance) then
		gwjinput.s_recentlyInstance:addObject(self)
	end
	params = params or {}
	local main_id = params.main_id
	self.m_mainNode = gui.get_node(main_id)
end

function clsGwjInputObject:getMainNode()
	return self.m_mainNode
end

function clsGwjInputObject:onTouch_(event)
end

return clsGwjInputObject
