local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test17 = gwjui.class("test17")

function test17:ctor()
end

function test17:onEnter()
	self.m_url = msg.url()
	gwjui.ScaleButton.new({
		main_id = "btn_install_apk",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickInstallApk()
	end)
end

function test17:onExit()
end

function test17:on_message(message_id, message, sender)
end

function test17:onClickInstallApk()
	local apkPath = "/mnt/sdcard/plmhd/xianliao.apk"
	local ok,ret = misc.installApk({
		path = apkPath,
	})
	gwjui.printf("gwjgwj,install apk,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

return test17
