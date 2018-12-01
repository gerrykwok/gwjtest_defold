local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test15 = gwjui.class("test15")

function test15:ctor()
end

function test15:onEnter()
	self.m_url = msg.url()
	gwjui.ScaleButton.new({
		main_id = "btn_share_system",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareSystem()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_share_wechat_sdk",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareWechatSDK()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_luatable_to_json",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:luaTableToJson()
	end)
end

function test15:onExit()
end

function test15:on_message(message_id, message, sender)
end

function test15:luaTableToJson()
	local key = function() end
	local ret = testext.test({
		abc = 1,
		def = "2",
		nickname = "郭伟基",
		[100] = 5,
		10,
		scale = 1.23,
		float = 1.7E308,
		pi = math.pi,
		huge = math.huge,
		callback = function()
		end,
		extras = {
			["android.intent.extra.TITLE"] = "the title",
			["android.intent.extra.TEXT"] = "the text",
			uid = 12345,
		},
		[key] = "aaa",
	})
	local huge = math.huge - 1
	print("huge=" .. tostring(huge))
	gwjui.dump(ret, "ret")
end

function test15:onClickShareSystem()
	gwjui.printf("gwjgwj,share system")
	local sysName = sys.get_sys_info().system_name
	if(sysName == "Android") then
		local Intent = {
			EXTRA_TITLE = "android.intent.extra.TITLE",
			EXTRA_TEXT = "android.intent.extra.TEXT",
			EXTRA_SUBJECT = "android.intent.extra.SUBJECT",
		}
		local pkgName = "com.tencent.mm"
		local clsName = "com.tencent.mm.ui.tools.ShareImgUI"
		pkgName = "org.xianliao"
		clsName = "org.sugram.base.LaunchActivity"
		local extras = {}
--		extras[Intent.EXTRA_SUBJECT] = "分享title"
--		extras[Intent.EXTRA_TEXT] = "分享text"--闲聊不用Intent.EXTRA_TITLE
		local ok, ret = wechat.shareWithIntent({
			pkgName = pkgName,
			clsName = clsName,
			streamPath = "/mnt/sdcard/myMahjongAvatar.png",
			mimeType = "image/*",
			extras = extras,
			callback = function()
			end,
		})
		gwjui.printf("gwjgwj,share with intent, ok=%s, ret=%s", tostring(ok), tostring(ret))
	elseif(sysName == "iPhone OS") then
	end
end

function test15:onClickShareWechatSDK()
end

return test15
