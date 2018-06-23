--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-06-20
-- Description: 单选按钮组

local gwjui = require("gwjui.gwjui_impl")
local UIPushButton = require("gwjui.UIPushButton")

local clsUICheckBoxButtonGroup = gwjui.class("UICheckBoxButtonGroup")

function clsUICheckBoxButtonGroup:ctor(params)
	params = params or {}
	local buttons = params.buttons or {}
	self.m_buttons = {}
	for i,btn in ipairs(buttons) do
		local button = UIPushButton.new({
			main_id = btn.id_normal,
		})
		:onButtonClicked(function()
			self:setSelected(btn.id)
		end)
		button.m_nodeNormal = gui.get_node(btn.id_normal)
		button.m_nodeSelected = gui.get_node(btn.id_selected)
		button.m_btnId = btn.id
		table.insert(self.m_buttons, button)
	end
end

function clsUICheckBoxButtonGroup:setSelected(id)
	for i,button in ipairs(self.m_buttons) do
		local selected = button.m_btnId == id
		gui.set_enabled(button.m_nodeNormal, not selected)
		gui.set_enabled(button.m_nodeSelected, selected)
	end
end

function clsUICheckBoxButtonGroup:getSelected()
	for i,button in ipairs(self.m_buttons) do
		local enable = gui.is_enabled(button.m_nodeSelected)
		if(enable) then
			return button.m_btnId
		end
	end
end

return clsUICheckBoxButtonGroup
