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

local info = sys.get_sys_info()
local sysName = info.system_name
--gwjui.printf("gwjgwj,system_name=%s", sysName)
if(sysName == "Android") then
	gwjui.platform = "android"
elseif(sysName == "iPhone OS") then
	gwjui.platform = "ios"
elseif(sysName == "Windows") then
	gwjui.platform = "windows"
elseif(sysName == "Darwin") then
	gwjui.platform = "mac"
end

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
