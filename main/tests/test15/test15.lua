local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test15 = gwjui.class("test15")

function test15:ctor()
	self.m_imageShare = nil
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
		main_id = "btn_share_wechat_sdk_text",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareWechatSDK_text()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_share_wechat_sdk_image",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareWechatSDK_image()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_share_wechat_sdk_music",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareWechatSDK_music()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_share_wechat_sdk_video",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareWechatSDK_video()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_share_wechat_sdk_webpage",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickShareWechatSDK_webpage()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_luatable_to_json",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		local key = function() end
		local ret = testext.luatableToJson({
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
		gwjui.printf("gwjgwj,json=%s", tostring(ret))
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_test_oc",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		for i=1,5 do
			local param = {
				testname = "test" .. i,
				param1 = "abc",
				param2 = 3,
--				callback = function()
--				end,
			}
			local ok, ret = testext.test_oc("MyClass", "test" .. i, param)
			gwjui.printf("gwjgwj,test oc %d, ok=%s, ret=%s", i, tostring(ok), tostring(ret))
		end
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_take_photo",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		local path = sys.get_save_file("myid", "takephoto.png")
		gwjui.printf("gwjgwj,path=%s", tostring(path))
		self.m_imageShare = path
		testext.takePhoto({
			fromeCamera = false,
			path = path,
			callback = function(script, res)
			end,
		})
	end)
end

function test15:onExit()
end

function test15:on_message(message_id, message, sender)
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
--		if(self.m_imageShare == nil) then
--			TipsBanner.show("请先takephoto")
--			return
--		end
		--图文分享
		local params = {
--			title = "the title",
			image = self.m_imageShare,
--			content = "the content",
		}
		--share link
--		local params = {
--			title = "the title",
--			image = self.m_imageShare,
--			url = "http://www.pconline.com.cn",
--		}
--[[
		params.excludeActivityType = {
			"com.apple.UIKit.activity.PostToFacebook",
			"com.apple.UIKit.activity.PostToTwitter",
			"com.apple.UIKit.activity.PostToWeibo",
			"com.apple.UIKit.activity.Message",
			"com.apple.UIKit.activity.Mail",
			"com.apple.UIKit.activity.Print",
			"com.apple.UIKit.activity.CopyToPasteboard",
			"com.apple.UIKit.activity.AssignToContact",
			"com.apple.UIKit.activity.SaveToCameraRoll",
			"com.apple.UIKit.activity.AddToReadingList",
			"com.apple.UIKit.activity.PostToFlickr",
			"com.apple.UIKit.activity.PostToVimeo",
			"com.apple.UIKit.activity.TencentWeibo",
			"com.apple.UIKit.activity.AirDrop",
			"com.apple.UIKit.activity.OpenInIBooks",

			"com.apple.mobilenotes.SharingExtension", "com.apple.mobileslideshow.StreamShareService",
			"com.tencent.mqq.ShareExtension",--QQ
			"com.apple.NanoTimeKitCompanion.CreateWatchFace",--创建表盘
			"com.apple.CloudDocsUI.AddToiCloudDrive",--存储到文件
			"com.alipay.iphoneclient.ExtensionSchemeShare",--支付宝
			"com.apple.reminders.RemindersEditorExtension",--提醒事项
		}
]]
		params.callback = function(script, res)
			gwjui.printf("gwjgwj,share res=%s", tostring(res))
		end
		local ok, ret = wechat.shareWithIosSystem(params)
		gwjui.printf("gwjgwj,share with ios system, ok=%s, ret=%s", tostring(ok), tostring(ret))
	end
end

function test15:onShareResult(res)
	gwjui.printf("gwjgwj,share result:%s", tostring(res))
	TipsBanner.show("分享结果:" .. tostring(res))
end

function test15:onClickShareWechatSDK_text()
	local WXSceneSession = 0
	local WXSceneTimeline = 1
	local WXSceneFavorite = 2
	local WXSceneSpecifiedContact = 3
	local params = {
		scene = WXSceneSession,
		--text
		type = "text",
		text = "my text",
		description = "my description",
		callback = function(script, res)
			self:onShareResult(res)
		end,
	}
	local ok,ret = wechat.shareWithSDK(params)
	gwjui.printf("gwjgwj,share with sdk, ok=%s, ret=%s", tostring(ok), tostring(ret))
end

function test15:onClickShareWechatSDK_image()
	local WXSceneSession = 0
	local WXSceneTimeline = 1
	local WXSceneFavorite = 2
	local WXSceneSpecifiedContact = 3
	local params = {
		scene = WXSceneSession,
		--image
		type = "image",
		image = "/mnt/sdcard/shareimage.jpg",
		callback = function(script, res)
			self:onShareResult(res)
		end,
	}
	if(sys.get_sys_info().system_name == "iPhone OS") then
		params.image = self.m_imageShare
	end
	local ok,ret = wechat.shareWithSDK(params)
	gwjui.printf("gwjgwj,share with sdk, ok=%s, ret=%s", tostring(ok), tostring(ret))
end

function test15:onClickShareWechatSDK_music()
	local WXSceneSession = 0
	local WXSceneTimeline = 1
	local WXSceneFavorite = 2
	local WXSceneSpecifiedContact = 3
	local params = {
		scene = WXSceneSession,
		--music
		type = "music",
		image = "/mnt/sdcard/share_icon3.png",
		url = "http://10.11.133.31/day%20tripper.mp3",
		title = "music title",
		description = "music description",
		callback = function(script, res)
			self:onShareResult(res)
		end,
	}
	if(sys.get_sys_info().system_name == "iPhone OS") then
		params.image = self.m_imageShare
	end
	local ok,ret = wechat.shareWithSDK(params)
	gwjui.printf("gwjgwj,share with sdk, ok=%s, ret=%s", tostring(ok), tostring(ret))
end

function test15:onClickShareWechatSDK_video()
	local WXSceneSession = 0
	local WXSceneTimeline = 1
	local WXSceneFavorite = 2
	local WXSceneSpecifiedContact = 3
	local params = {
		scene = WXSceneSession,
		--video
		type = "video",
		title = "video title",
		description = "video description",
		url = "http://10.11.133.31/initial-d-26.mp4",
		image = "/mnt/sdcard/share_icon3.png",
		callback = function(script, res)
			self:onShareResult(res)
		end,
	}
	if(sys.get_sys_info().system_name == "iPhone OS") then
		params.image = self.m_imageShare
	end
	local ok,ret = wechat.shareWithSDK(params)
	gwjui.printf("gwjgwj,share with sdk, ok=%s, ret=%s", tostring(ok), tostring(ret))
end

function test15:onClickShareWechatSDK_webpage()
	local WXSceneSession = 0
	local WXSceneTimeline = 1
	local WXSceneFavorite = 2
	local WXSceneSpecifiedContact = 3
	local params = {
		scene = WXSceneSession,
		--webpage
		type = "webpage",
		title = "the title",
		description = "the description",
		url = "https://www.pconline.com.cn",
		image = "/mnt/sdcard/share_icon3.png",

		callback = function(script, res)
			self:onShareResult(res)
		end,
	}
	if(sys.get_sys_info().system_name == "iPhone OS") then
		params.image = self.m_imageShare
	end
	local ok,ret = wechat.shareWithSDK(params)
	gwjui.printf("gwjgwj,share with sdk, ok=%s, ret=%s", tostring(ok), tostring(ret))
end

return test15
