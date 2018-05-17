local MiscUtil = require("main.common.MiscUtil")
local timer = require("main.common.timer")

local TipsBanner = {}

TipsBanner.s_allRunningTipsUrl = {}

function TipsBanner.show(str)
	for i,url in ipairs(TipsBanner.s_allRunningTipsUrl) do
		msg.post(url, "deleteTipsBanner")
	end
	TipsBanner.s_allRunningTipsUrl = {}
	
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

function TipsBanner.init()
	local url = msg.url()
	table.insert(TipsBanner.s_allRunningTipsUrl, url)
end

function TipsBanner.final()
	local url = msg.url()
	MiscUtil.table.removebyvalue(TipsBanner.s_allRunningTipsUrl, url)
end

return TipsBanner
