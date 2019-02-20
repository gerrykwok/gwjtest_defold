local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test25 = gwjui.class("test25")

function test25:ctor()
end

function test25:onEnter()
	gwjui.ScaleButton.new({
		main_id = "btn_start",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickLocationStart()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_stop",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickLocationStop()
	end)
	local res = gaode.locationInit()
	gwjui.printf("location init,res=%s", tostring(res))
end

function test25:onExit()
end

function test25:on_message(message_id, message, sender)
end

function test25:onClickLocationStart()
	local ok,res = gaode.locationStart({
		callback = function(script, res)
			self:locationCallback(res)
		end,
	})
	gwjui.printf("location start,ok=%s,res=%s", tostring(ok), tostring(res))
end

function test25:onClickLocationStop()
	local ok,res = gaode.locationStop()
	gwjui.printf("location stop,ok=%s,res=%s", tostring(ok), tostring(res))
end

function test25:locationCallback(res)
	gwjui.printf("location:%s", tostring(res))
	local t = json.decode(res)
	local str
	if(t.result == "success") then
		str = string.format("经度:%f\n纬度:%f\n省:%s\n市:%s", t.longitude, t.latitude, t.province, t.city)
	else
		str = string.format("%s,code=%d,%s", t.result, t.errCode or 0, t.errInfo or "")
	end
	local node = gui.get_node("location_info")
	gui.set_text(node, str)
end

return test25
