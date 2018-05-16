require("main.common.MiscUtil")
local timer = require("main.common.timer")

local TipsBanner = {}

TipsBanner.s_urlCurrent = nil

function TipsBanner.show(str)
	if(TipsBanner.s_urlCurrent) then
		msg.post(TipsBanner.s_urlCurrent, "deleteTipsBanner")
		TipsBanner.s_urlCurrent = nil
	end
	
	local ids = collectionfactory.create("#tipsbanner")
	local firstId = nil
	if(ids) then
		for k,v in pairs(ids) do
			firstId = v
			break
		end
		ccprint("firstId=%s", tostring(firstId))
		TipsBanner.s_urlCurrent = firstId
		msg.post(firstId, "setText", {text=str})
		msg.post(firstId, "delayDestroy", {delay=2})
	end
end

function TipsBanner.final()
	local url = msg.url()
	ccprint("url in final:%s", tostring(url))
	TipsBanner.s_urlCurrent = nil
end

return TipsBanner
