local gwjui = require("gwjui.gwjui_impl")

gwjui.input = require("gwjui.gwjinput")
gwjui.DarkenButton = require("gwjui.DarkenButton")
gwjui.InputObject = require("gwjui.InputObject")
gwjui.PushButton = require("gwjui.PushButton")
gwjui.ScaleButton = require("gwjui.ScaleButton")
gwjui.UIListView = require("gwjui.UIListView")
gwjui.UIPushButton = require("gwjui.UIPushButton")
gwjui.UIInput = require("gwjui.UIInput")

function gwjui.scheduleUpdate(func)
	if(gwjui.input.s_recentlyInstance) then
		gwjui.input.s_recentlyInstance:scheduleUpdate(func)
	end
end

function gwjui.unscheduleUpdate(func)
	if(gwjui.input.s_recentlyInstance) then
		gwjui.input.s_recentlyInstance:unscheduleUpdate(func)
	end
end

return gwjui
