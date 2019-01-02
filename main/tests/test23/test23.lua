local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test23 = gwjui.class("test23")

function test23:ctor()
end

function test23:onEnter()
	self.m_url = msg.url()

	gwjui.ScaleButton.new({
		main_id = "btn_download",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickDownload()
	end)
end

function test23:onExit()
end

function test23:on_message(message_id, message, sender)
end

function test23:onClickDownload()
	gui.set_text(gui.get_node("text_progress"), "下载中")
	
	local url = "http://www.xsjplm.com:7654/?c=Index&a=download_info&app_id=XYd0ogCwfB4wYCqdikYooVe"
	url = "http://qnweb.xsjplm.com/mj_beta_201806051000_1.apk"
	http.request(url, "GET", function(self, _, response)
		gwjui.dump(response, "download response")
		local status = response.status
		local resp = response.response
		local headers = response.headers
		local filesize = string.len(resp)
		gwjui.printf("filesize=%d", filesize)
		local text = string.format("下载完毕,status=%s,filesize=%d", tostring(status), filesize)
		local filepath = ""
		local filename = "mj_beta_201806051000_1.apk"
		local sysname = sys.get_sys_info().system_name
		gwjui.printf("sysname=%s", sysname)
		if(sysname == "Android") then
			filepath = "/mnt/sdcard/" .. filename
		elseif(sysname == "iPhone OS") then
			filepath = sys.get_save_file("gwjtest", filename)
		elseif(sysname == "Windows") then
			filepath = "e:/" .. filename
		elseif(sysname == "Darwin") then
			filepath = sys.get_save_file("gwjtest", filename)
		end
		gwjui.printf("gwjgwj,save to %s", filepath)
		local f = io.open(filepath, "wb")
		if(f) then
			f:write(resp)
			f:close()
		end
		gui.set_text(gui.get_node("text_progress"), text)
	end)
end

return test23
