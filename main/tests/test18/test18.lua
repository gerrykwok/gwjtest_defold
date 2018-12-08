local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test18 = gwjui.class("test18")

function test18:ctor()
end

function test18:onEnter()
	self.m_url = msg.url()
	gwjui.ScaleButton.new({
		main_id = "btn_set_userid",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onSetUserId()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_add_extra_data",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onAddExtraData()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_send_error",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onSendError()
	end)
end

function test18:onExit()
end

function test18:on_message(message_id, message, sender)
end

function test18:onSetUserId()
	local ok,ret = crasheye.setUserId("gwjgwj123456")
	gwjui.printf("gwjgwj,ser userid,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

function test18:onAddExtraData()
	local ok,ret = crasheye.addExtraData({
		category = 123,
		nickname = "oops!",
		timestamp = os.time(),
	})
	gwjui.printf("gwjgwj,add extra data,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

function test18:onSendError()
	local ok,ret = crasheye.sendScriptError({
		title = "error title",
		content = "error content",
	})
	gwjui.printf("gwjgwj,send script error,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

return test18
