local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")
local LoadingView = require("main.common.loading.LoadingView")

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
	gwjui.ScaleButton.new({
		main_id = "btn_httpreq",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickHttpReq()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_https",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickHttps()
	end)
end

function test23:onExit()
	if(self.m_request) then
		self.m_request:cancel()
		self.m_request = nil
	end
end

function test23:setLoadingView(flag)
	if(flag) then
		if(self.m_loading == nil) then
			self.m_loading = LoadingView.show()
		end
	else
		if(self.m_loading) then
			self.m_loading:close()
			self.m_loading = nil
		end
	end
end

function test23:on_message(message_id, message, sender)
end

function test23:getOutputPath(filename)
	local filepath = ""
	local sysname = sys.get_sys_info().system_name
	if(sysname == "Android") then
		filepath = "/mnt/sdcard/" .. filename
	elseif(sysname == "iPhone OS") then
		filepath = sys.get_save_file("gwjtest", filename)
	elseif(sysname == "Windows") then
		filepath = "e:/" .. filename
	elseif(sysname == "Darwin") then
		filepath = sys.get_save_file("gwjtest", filename)
	end
	return filepath
end

function test23:onClickDownload()
	if(self.m_downloading) then
		TipsBanner.show("正在下载中")
		return
	end

	self.m_downloading = true
	gui.set_text(gui.get_node("text_progress"), "下载中")
	
	local url = "http://www.xsjplm.com:7654/?c=Index&a=download_info&app_id=XYd0ogCwfB4wYCqdikYooVe"
	url = "http://qnweb.xsjplm.com/mj_beta_201806051000_1.apk"
	http.request(url, "GET", function(script, _, response)
		gwjui.dump(response, "download response")
		local status = response.status
		local resp = response.response
		local headers = response.headers
		local filesize = string.len(resp)
		gwjui.printf("filesize=%d", filesize)
		local text = string.format("下载完毕,status=%s,filesize=%d", tostring(status), filesize)
		local filepath = self:getOutputPath("mj_beta_201806051000_1.apk")
		gwjui.printf("gwjgwj,save to %s", filepath)
		local f = io.open(filepath, "wb")
		if(f) then
			f:write(resp)
			f:close()
		else
			gwjui.printf("cannot open %s for writing", filepath)
		end
		gui.set_text(gui.get_node("text_progress"), text)
		self.m_downloading = false
	end)
end

function test23:getFilenameFromURL(url)
	local filename = ""
	local len = string.len(url)
	for i=len,1,-1 do
		if(string.sub(url, i, i) == "/") then
			filename = string.sub(url, i+1)
			break
		end
	end
	return filename
end

function test23:onClickHttpReq()
	local url = "http://qnweb.xsjplm.com/mj_beta_201806051000_1.apk"
	url = "http://10.11.133.31/day%20tripper.mp3"
--	url = "http://10.11.133.31/gdmj-release.apk"
--	url = "http://10.11.133.32/invalid.aaa"
--	url = "http://dldir1.qq.com/weixin/android/weixin703android1400.apk"
	local req
	local dltotal = -1
	gwjui.printf("begin to download %s", url)
	self:setLoadingView(true)
	req = httpreq.HTTPRequest:create(function(script, res)
		local t = json.decode(res)

		if(t.name == "progress") then
			if(t.dltotal ~= dltotal) then
				gwjui.printf("lua:progress: %d/%d", t.dltotal, t.total)
				dltotal = t.dltotal
			end
			gui.set_text(gui.get_node("text_progress"), string.format("进度：%d/%d", t.dltotal, t.total))
			local size = gui.get_size(gui.get_node("progress1"))
			size.x = size.x * t.dltotal / t.total
			gui.set_size(gui.get_node("progress2"), size)
			return
		end

		self.m_request = nil
		gwjui.printf("lua:httpreq callback,res=%s", tostring(res))
		self:setLoadingView(false)
		if(t.name == "completed") then
			local statusCode = req:getResponseStatusCode()
			if(statusCode == 200) then
				local len = req:getResponseDataLength()
				local str = req:getResponseString()
				local respData = req:getResponseDataLua()
				gwjui.printf("response size=%d", len)
				gwjui.printf("response string=%s", str)
				gwjui.printf("response data=%s", respData)
				local filepath = self:getOutputPath(self:getFilenameFromURL(url))
--				gwjui.printf("save to %s", filepath)
--				req:saveResponseData(filepath)
				local f = io.open(filepath, "wb")
				if(f) then
					f:write(respData)
					f:close()
					TipsBanner.show("create success:" .. filepath)
				else
					gwjui.printf("cannot create %s", filepath)
					TipsBanner.show("cannot create " .. filepath)
				end
			else
				TipsBanner.showf("statusCode=%s", tostring(statusCode))
			end
		elseif(t.name == "failed") then
		end
	end, url, "get")
	self.m_request = req
--	req:setConnectTimeout(3)
--	req:setReadTimeout(300)
	req:setTimeout(0)
	gwjui.dump(req, "req")
	req:start()
end

function test23:onClickHttps()
	local url = "https://mvlpthik01.boyaagame.com/mobile.php"
--	url = "http://10.11.133.34/invalid.aaa"
	local req
	self:setLoadingView(true)
	req = httpreq.HTTPRequest:create(function(script, res)
		local t = json.decode(res)
		if(t.name ~= "progress") then
			gwjui.printf("lua:httpreq callback,res=%s", tostring(res))
			self:setLoadingView(false)
		end
		if(t.name == "progress") then
		elseif(t.name == "completed") then
			local statusCode = req:getResponseStatusCode()
			gwjui.printf("statusCode=%d", statusCode)
			if(statusCode == 200) then
				local len = req:getResponseDataLength()
				local str = req:getResponseString()
				local respData = req:getResponseDataLua()
				gwjui.printf("response size=%d", len)
				gwjui.printf("response string=%s", str)
				gwjui.printf("response data=%s", respData)
			else
				local str = string.format("statusCode=%s", tostring(statusCode))
				TipsBanner.show(str)
			end
		elseif(t.name == "failed") then
			local str = string.format("request failed,code=%s,msg=%s", tostring(req:getErrorCode()), tostring(req:getErrorMessage()))
			gwjui.printf(str)
			TipsBanner.show(str)
		end
	end, url, "post")
	req:setTimeout(0)
	req:start()
end

return test23
