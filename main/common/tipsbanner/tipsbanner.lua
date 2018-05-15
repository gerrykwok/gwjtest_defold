require("main.common.MiscUtil")
local timer = require("main.common.timer")

local TipsBanner = {}

function TipsBanner.show(str)
	local ids = collectionfactory.create("#tipsbanner")
	local firstId = nil
	if(ids) then
		for k,v in pairs(ids) do
			firstId = v
			break
		end
		msg.post(firstId, "setText", {text=str})
		msg.post(firstId, "delayDestroy", {delay=2})
	end
end

return TipsBanner
