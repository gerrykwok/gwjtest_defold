local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test20 = gwjui.class("test20")

function test20:ctor()
end

function test20:onEnter()
	self.m_url = msg.url()

	gwjui.ScaleButton.new({
		main_id = "btn_umeng_event0",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onSendUmengEvent0()
	end)
	
	gwjui.ScaleButton.new({
		main_id = "btn_umeng_event1",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onSendUmengEvent1()
	end)

end

function test20:onExit()
end

function test20:on_message(message_id, message, sender)
end

function test20:onSendUmengEvent0()
	local ok,ret = umeng.sendEvent({
		name = "myevent0",
	})
	gwjui.printf("gwjgwj,send umeng event,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

function test20:onSendUmengEvent1()
	local ok,ret = umeng.sendEvent({
		name = "myevent",
		args = {
			timestamp = os.time(),
			nickname = "郭伟基",
			is_defold = true,
		},
	})
	gwjui.printf("gwjgwj,send umeng event,ok=%s,ret=%s", tostring(ok), tostring(ret))
end

return test20
