local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test24 = gwjui.class("test24")

function test24:ctor()
end

function test24:onEnter()
	self.m_url = msg.url()

	local sdk = misc.androidGetTargetSdkVersion()
	TipsBanner.show(string.format("targetSdkVersion:%s(%s)", tostring(sdk), type(sdk)))
	local info = sys.get_sys_info()
	gwjui.dump(info, "sys info")

	gwjui.ScaleButton.new({
		main_id = "btn_detect",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickDetect()
	end)

	gwjui.ScaleButton.new({
		main_id = "btn_request",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickRequest()
	end)

	gwjui.ScaleButton.new({
		main_id = "btn_savefile_directly",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickSaveFileDirectly()
	end)

	gwjui.ScaleButton.new({
		main_id = "btn_savefile",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickSaveFile()
	end)
end

function test24:onExit()
end

function test24:on_message(message_id, message, sender)
end

function test24:onClickDetect()
	local permission = "android.permission.WRITE_EXTERNAL_STORAGE"
	local res = misc.androidCheckPermission(permission)
	gwjui.printf("%s:%s", permission, tostring(res))
	local str = string.format("%s:%s", permission, tostring(res))
	TipsBanner.show(str)
end

function test24:onClickRequest()
	local ok,res = misc.androidRequestPermissions({
		permissions = {
			"android.permission.WRITE_EXTERNAL_STORAGE",
			"android.permission.CALL_PHONE",
			"android.permission.CAMERA",
		},
		callback = function(script, res)
			local t = json.decode(res)
			local str = ""
			for k,v in pairs(t) do
				if(str ~= "") then
					str = str .. "\n"
				end
				str = str .. k .. ":" .. tostring(v)
			end
			TipsBanner.show(str)
		end,
	})
	gwjui.printf("request permissions,ok=%s,res=%s", tostring(ok), tostring(res))
end

function test24:onClickSaveFileDirectly()
	self:doSaveFile(true)
end

function test24:onClickSaveFile()
	if(not self:doSaveFile(false)) then
		if(misc.androidGetTargetSdkVersion() < 23) then
			self:gotoAppSetting()
		else
			local permission = "android.permission.WRITE_EXTERNAL_STORAGE"
			misc.androidRequestPermissions({
				permissions = {
					permission,
				},
				callback = function(script, res)
					local t = json.decode(res)
					if(t[permission] == "granted") then
						self:doSaveFile(true)
					else
						self:gotoAppSetting()
					end
				end,
			})
		end
	else
		TipsBanner.show("保存文件成功1")
	end
end

function test24:gotoAppSetting()
	local res = misc.androidGotoAppSetting()
	gwjui.printf("goto app setting,res=%s", tostring(res))
end

function test24:doSaveFile(showTips)
	local success = false
	local path = "/mnt/sdcard/gwjtest_output.txt"
	local f = io.open(path, "w")
	if(f) then
		f:write("gwjtest")
		f:close()
		if(showTips) then
			TipsBanner.show("保存文件成功")
		end
		success = true
	else
		if(showTips) then
			TipsBanner.show("保存文件失败")
		end
		success = false
	end
	return success
end

return test24
