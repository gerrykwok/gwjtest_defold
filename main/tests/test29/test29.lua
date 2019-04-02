local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test29 = gwjui.class("test29")

function test29:ctor()
	self.m_anchor = gui.get_node("anchor")
end

function test29:onEnter()
	gui.set_enabled(self.m_anchor, false)
	gwjui.ScaleButton.new({
		main_id = "button1",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		TipsBanner.show("button1 clicked")
	end)
	gwjui.ScaleButton.new({
		main_id = "button2",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		TipsBanner.show("button2 clicked")
	end)
end

function test29:onExit()
end

function test29:on_message(message_id, message, sender)
end

function test29:on_input(action_id, action)
--	gwjui.dump(action, "actionid:"..tostring(action_id))
	if(action_id == nil) then return end
	if(action.pressed) then--began
		self.m_xDown = action.x
		self.m_yDown = action.y
		gwjui.printf("gwjgwj,began")
		gui.set_enabled(self.m_anchor, true)
		gui.set_position(self.m_anchor, vmath.vector3(action.x, action.y, 0))
	elseif(action.released) then--ended
		gwjui.printf("gwjgwj,ended")
		gui.set_enabled(self.m_anchor, false)
	else
		if(action.x ~= self.m_xDown or action.y ~= self.m_yDown) then--moved
			self.m_xDown = action.x
			self.m_yDown = action.y
--			gwjui.dump(action, "moved")
			gwjui.printf("gwjgwj,moved:%d,%d %d,%d", action.x, action.y, action.screen_x, action.screen_y)
			gui.set_position(self.m_anchor, vmath.vector3(action.x, action.y, 0))
		end
	end
end

return test29
