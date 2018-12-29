local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test22 = gwjui.class("test22")

function test22:ctor()
end

function test22:onEnter()
	self.m_url = msg.url()

	gwjui.ScaleButton.new({
		main_id = "btn_load_products",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onLoadProducts()
	end)
end

function test22:onExit()
end

function test22:on_message(message_id, message, sender)
end

function test22:onLoadProducts()
	appleiap.loadProducts({
		productsId = {
			"qyq_diamond2_6",
			"qyq_diamond_12",
			"qyq_diamond_30",
			"qyq_diamond_60",
			"qyq_diamond_98",
			"qyq_diamond_153",
			"qyq_diamond_198",
			"qyq_diamond_298",
		},
		callback = function(script, res)
		end,
	})
end

return test22
