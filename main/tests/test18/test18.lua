local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test18 = gwjui.class("test18")

test18.has_set_error_handle = false

function test18:ctor()
	if(not test18.has_set_error_handle) then
		test18.has_set_error_handle = true
		sys.set_error_handler(gwjui.handler(self, self.onLuaError))
	end
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
	gwjui.ScaleButton.new({
		main_id = "btn_raise_error",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onRaiseError()
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

function test18:onLuaError(source, message, traceback)
--	gwjui.printf("gwjgwj,error,source=%s", tostring(source))
--	gwjui.printf("gwjgwj,error,message=%s", tostring(message))
--	gwjui.printf("gwjgwj,error,traceback=%s", tostring(traceback))
	crasheye.sendScriptError({
		title = message,
		content = traceback,
	})
end

function test18:onRaiseError()
	self:raiseError1()
end

function test18:raiseError1()
	self:raiseError2()
end

function test18:raiseError2()
	error("gwj error")
end

return test18
