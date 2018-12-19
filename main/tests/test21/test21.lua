local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test21 = gwjui.class("test21")

test21.initcallback = false

function test21:ctor()
	if(not test21.initcallback) then
		test21.initcallback = true
		agora.initCallback(function(script, res)
			local t = json.decode(res)
			gwjui.dump(t, "agora callback")
		end)
	end
end

function test21:onEnter()
	self.m_url = msg.url()
	self.m_userId = math.random(100, 200)--随机生成一个UID

	gwjui.ScaleButton.new({
		main_id = "btn_join_channel",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickJoinChannel()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_leave_channel",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickLeaveChannel()
	end)
	
end

function test21:onExit()
end

function test21:on_message(message_id, message, sender)
end

function test21:onClickJoinChannel()
	local channelName = "testChannel"
	agora.joinChannel({
		channel = channelName,
		uid = self.m_userId,
	})
end

function test21:onClickLeaveChannel()
	agora.leaveChannel()
end

return test21
