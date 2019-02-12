local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test21 = gwjui.class("test21")

test21.initcallback = false

function test21:ctor()
	if(not test21.initcallback) then
		test21.initcallback = true
--		agora.initCallback(function(script, res)
--			local t = json.decode(res)
--			gwjui.dump(t, "agora callback")
--		end)
	end
end

function test21:onEnter()
	self.m_url = msg.url()
	math.randomseed(os.time())
	self.m_userId = math.random(10000, 30000)--随机生成一个UID
	gui.set_text(gui.get_node("text_userid"), "userId:" .. self.m_userId)

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
	gwjui.ScaleButton.new({
		main_id = "btn_current_channel",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickCurrentChannel()
	end)
end

function test21:onExit()
end

function test21:on_message(message_id, message, sender)
end

function test21:onClickJoinChannel()
	local channelName = "testChannel"
	local function doJoinChannel()
		agora.joinChannel({
			channel = channelName,
			uid = self.m_userId,
		})
	end
	if(gwjui.platform == "android") then
		local permission = "android.permission.RECORD_AUDIO"
		local res = misc.androidCheckPermission(permission)
		TipsBanner.show("res=" .. tostring(res))
		if(res == "granted") then
			doJoinChannel()
		else
			if(misc.androidGetTargetSdkVersion() < 23) then
				TipsBanner.show("请在应用的权限设置中打开录音权限")
				timer.delay(1, false, function()
					misc.androidGotoAppSetting()
				end)
			else
				misc.androidRequestPermissions({
					permissions = {
						permission,
					},
					callback = function(script, res)
						local t = json.decode(res)
						if(t[permission] == "granted") then
							doJoinChannel()
						else
							TipsBanner.show("请在应用的权限设置中打开录音权限")
							timer.delay(1, false, function()
								misc.androidGotoAppSetting()
							end)
						end
					end,
				})
			end
		end
	else
		doJoinChannel()
	end
end

function test21:onClickLeaveChannel()
	agora.leaveChannel()
end

function test21:onClickCurrentChannel()
	local channel = agora.getCurrentChannel()
	TipsBanner.show("当前频道:" .. tostring(channel))
end

return test21
