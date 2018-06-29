local gwjui = require("gwjui.gwjui")

local clsLoadingView = gwjui.class("LoadingView")
clsLoadingView.s_instCreating = nil

function clsLoadingView.show(params)
	local view = clsLoadingView.new(params)
	return view
end

function clsLoadingView:ctor(params)
	params = params or {}
	clsLoadingView.s_instCreating = self
	local factoryName = params.factory or "#loading"
	local id = factory.create(factoryName)
	self.m_id = id
	msg.post(id, "on_create")
end

function clsLoadingView:onEnter()
	--mask
	local mask = gwjui.UIPushButton.new({
		main_id = "mask",
	})

	local juhua = gui.get_node("juhua")
	gui.animate(juhua, "rotation.z", -360, gui.EASING_LINEAR, 2, 0, nil, gui.PLAYBACK_LOOP_FORWARD)
	local timeStart = os.clock()
	local num_dot = 0
	self.m_tickFunc = function(dt)
		local now = os.clock()
		local elapsed = now - timeStart
		local numDot = math.modf(elapsed / 0.5 % 4)
		if(numDot ~= num_dot) then
			num_dot = numDot
			local node = gui.get_node("text")
			local str = "请稍候"
			for i=1,numDot do
				str = str .. "."
			end
			gui.set_text(node, str)
		end
	end
	gwjui.scheduleUpdate(self.m_tickFunc)
end

function clsLoadingView:onExit()
	gwjui.unscheduleUpdate(self.m_tickFunc)
end

function clsLoadingView:close()
	msg.post(self.m_id, "deleteSelf")
end

return clsLoadingView
