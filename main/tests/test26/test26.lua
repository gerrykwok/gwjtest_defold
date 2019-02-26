local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test26 = gwjui.class("test26")

function test26:ctor()
end

function test26:onEnter()
--	local path = sys.get_save_file("", "")
--	TipsBanner.show("app support folder:" .. path)

--	for k,v in pairs(lfs) do
--		gwjui.printf("lfs.%s = %s (%s)", k, tostring(v), type(v))
--	end

	gwjui.ScaleButton.new({
		main_id = "btn_check_manifest",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:checkManifest()
	end)
	
	gwjui.ScaleButton.new({
		main_id = "btn_jump_collection",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:jumpCollection()
	end)
end

function test26:onExit()
end

function test26:on_message(message_id, message, sender)
	gwjui.printf("messageid=%s", tostring(message_id))
	if message_id == hash("proxy_loaded") then
		msg.post(sender, "init")--这里不调用都可以
		msg.post(sender, "enable")
	end
end

local g_localVerFile = "version.lua"

function test26:getLocalVersion()
	local version = {build=0}
	local path = sys.get_save_file(sys.get_config("project.title"), g_localVerFile)
	local file = io.open(path, "rb")
	if(file) then
		local str = file:read("*a")
		file:close()
		local fn = loadstring(str)
		if(fn) then
			version = fn()
		end
	end
	if(type(version) ~= "table") then
		version = {build=0}
	end
	return version
end

function test26:saveLocalVersion(version)
	local path = sys.get_save_file(sys.get_config("project.title"), g_localVerFile)
	local file = io.open(path, "wb")
	if(file) then
		local str = string.format([[
		local version = {
			build = %d,
		}
		return version
		]], version.build)
		file:write(str)
		file:close()
	end
end

function test26:getUpdateServerHost()
	return "10.11.133.31"
end

function test26:checkManifest()
	local remoteVer = nil
	local localVer = self:getLocalVersion()
	local url = string.format("http://%s/liveupdate/%s/version.lua", self:getUpdateServerHost(), sys.get_config("project.title"))
	http.request(url, "GET", function(script, id, response)
		local download = false
		if(response.status == 200 or response.status == 304) then
			local resp = response.response
			local fn = loadstring(resp)
			if(fn) then
				local version = fn()
				gwjui.dump(version, "version")
				gwjui.dump(localVer, "local version")
				if(version.build > localVer.build) then
					remoteVer = version
					download = true
				end
			end
		end
		if(download) then
			local url = string.format("http://%s/liveupdate/%s/liveupdate.game.dmanifest", self:getUpdateServerHost(), sys.get_config("project.title"))
			http.request(url, "GET", function(script, id, response)
--				gwjui.printf("status=%d", response.status)
				if(response.status == 200 or response.status == 304) then
					resource.store_manifest(response.response, function(script, status)
						if(status == resource.LIVEUPDATE_OK) then
							TipsBanner.show("store manifest success")
							self:saveLocalVersion(remoteVer)
						else
							TipsBanner.show("store manifest failed")
						end
					end)
				else
					TipsBanner.show("download manifest failed")
				end
			end)
		else
			gwjui.printf("need not download manifest")
			TipsBanner.show("need not download manifest")
		end
	end)
end

function test26:jumpCollection()
	local missing_resources = collectionproxy.missing_resources("#jump")
	local count_pending = #missing_resources
	gwjui.printf("gwjgwj,count_pending=%s", tostring(count_pending))
	for _,res_hash in ipairs(missing_resources) do
		gwjui.printf("gwjgwj,missing:%s(%s)", tostring(res_hash), type(res_hash))
		local url = string.format("http://%s/liveupdate/%s/%s", self:getUpdateServerHost(), sys.get_config("project.title"), tostring(res_hash))
		http.request(url, "GET", function(script, id, response)
			if(response.status == 200 or response.status == 304) then
				resource.store_resource(resource.get_current_manifest(), response.response, res_hash, function(self, hexdigest, status)
					gwjui.printf("gwjgwj,store resource,status=%s", tostring(status))
					if(status) then
						count_pending = count_pending - 1
						if(count_pending <= 0) then
							msg.post("#jump", "async_load")
						end
					end
				end)
			end
		end)
	end
	if(count_pending <= 0) then
		msg.post("#jump", "async_load")
	end
end

return test26
