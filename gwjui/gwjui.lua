local gwjui = require("gwjui.gwjui_impl")

gwjui.input = require("gwjui.gwjinput")
gwjui.DarkenButton = require("gwjui.DarkenButton")
gwjui.InputObject = require("gwjui.InputObject")
gwjui.PushButton = require("gwjui.PushButton")
gwjui.ScaleButton = require("gwjui.ScaleButton")
gwjui.UIListView = require("gwjui.UIListView")
gwjui.UIPushButton = require("gwjui.UIPushButton")
gwjui.UIInput = require("gwjui.UIInput")
gwjui.TextureCache = require("gwjui.TextureCache")
gwjui.UICheckBoxButtonGroup = require("gwjui.UICheckBoxButtonGroup")

function gwjui.scheduleUpdate(func)
	local input = gwjui.input.getCurrentInput()
	if(input) then
		input:scheduleUpdate(func)
	end
end

function gwjui.unscheduleUpdate(func)
	local input = gwjui.input.getCurrentInput()
	if(input) then
		input:unscheduleUpdate(func)
	end
end

return gwjui
