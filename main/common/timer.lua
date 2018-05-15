local timer = {}
timer.m_allTimer = {}

function timer.performWithDelayGlobal(func, delay)
	table.insert(timer.m_allTimer, {
		func = func,
		delay = delay,
		elapsed = 0,
	})
end

function timer.update(dt)
	for i,t in ipairs(timer.m_allTimer) do
		t.elapsed = t.elapsed + dt
		if(t.elapsed >= t.delay) then
			t.func()
			t.delete = true
		end
	end
	for j=#timer.m_allTimer,1,-1 do
		local t = timer.m_allTimer[j]
		t.elapsed = t.elapsed + dt
		if(t.elapsed >= t.delay) then
			local func = t.func
			table.remove(timer.m_allTimer, j)
			func()
		end
	end
end

return timer
