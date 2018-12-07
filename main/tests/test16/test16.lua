local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test16 = gwjui.class("test16")

function test16:ctor()
end

function test16:onEnter()
	self.m_url = msg.url()
	gwjui.ScaleButton.new({
		main_id = "btn_wechat_pay",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickPay()
	end)
end

function test16:onExit()
end

function test16:on_message(message_id, message, sender)
end

function test16:onClickPay()
	gwjui.printf("click pay")
	local url = "https://wxpay.wxutil.com/pub_v2/app/app_pay.php"
	http.request(url, "POST", function(self, _, response)
--		gwjui.dump(response, "response", 1000)
		if(response.status == 200) then
			local resp = json.decode(response.response)
			gwjui.dump(resp, "resp")
			if(sys.get_sys_info().system_name == "Android") then
				local ok,ret = wechat.makePurchase({
					appid = resp.appid,
					partnerid = resp.partnerid,
					prepayid = resp.prepayid,
					package = resp.package,
					noncestr = resp.noncestr,
					timestamp = resp.timestamp,
					sign = resp.sign,
					callback = function(script, res)
						TipsBanner.show("pay result:" .. tostring(res))
					end,
				})
				gwjui.printf("gwjgwj,make wechat purchase,ok=%s,ret=%s", tostring(ok), tostring(ret))
			end
		else
			TipsBanner.show("请求下单失败")
		end
	end)
end

return test16
