local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test22 = gwjui.class("test22")

function test22:ctor()
end

function test22:onEnter()
	self.m_url = msg.url()

	appleiap.init({
		callback = function(script, res)
			--gwjui.printf("transaction callback,res=%s", tostring(res))
			self:onTransactionCallback(json.decode(res))
		end,
	})

	local can = appleiap.canMakePurchases()
	gwjui.printf("gwjgwj,can make purchases:%s", tostring(can))

	self.m_listProducts = gwjui.UIListView.createInstance({
		main_id = "product_list",
		direction = gwjui.UIListView.DIRECTION_HORIZONTAL,
	})
	:onTouch(function(event)
		if(event.name == "clicked") then
			self:makePurchase(event.item.m_productId)
		end
	end)

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
			"my_invalid_product1",
			"my_invalid_product2",
			"qyq_diamond_198",
			"qyq_diamond_298",
		},
		callback = function(script, res)
			self:onLoadProductResult(json.decode(res))
		end,
	})
	if(sys.get_sys_info().system_name ~= "iPhone OS") then--not ios
		local result = {
			invalidProductsId = {
				"my_invalid_product2",
				"my_invalid_product1",
			},
			products = {
				{
					localizedDescription = "花费6元购买6元钻石",
					localizedTitle       = "6元钻石",
					price                = 6,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond2_6",
				},
				{
					localizedDescription = "花费12元购买12元钻石",
					localizedTitle       = "12元钻石",
					price                = 12,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond_12",
				},
				{
					localizedDescription = "花费153元购买153元钻石",
					localizedTitle       = "153元钻石",
					price                = 153,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond_153",
				},
				{
					localizedDescription = "花费198元购买198元钻石",
					localizedTitle       = "198元钻石",
					price                = 198,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond_198",
				},
				{
					localizedDescription = "花费298元购买298元钻石",
					localizedTitle       = "298元钻石",
					price                = 298,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond_298",
				},
				{
					localizedDescription = "花费30元购买30元钻石",
					localizedTitle       = "30元钻石",
					price                = 30,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond_30",
				},
				{
					localizedDescription = "花费60元购买60元钻石",
					localizedTitle       = "60元钻石",
					price                = 60,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond_60",
				},
				{
					localizedDescription = "花费98元购买98元钻石",
					localizedTitle       = "98元钻石",
					price                = 98,
					priceLocale          = "zh_CN@currency=CNY",
					productIdentifier    = "qyq_diamond_98",
				},
			},
			success = true,
		}
		self:onLoadProductResult(result)
	end
end

function test22:onLoadProductResult(result)
	gwjui.dump(result, "load product")
	self.m_listProducts:removeAllItems()
	if(result.success) then
		for _,product in ipairs(result.products) do
			gwjui.printf("add item")
			local item = self.m_listProducts:newItem()
			local rootName = "product_item"
			local clones = gui.clone_tree(gui.get_node(rootName))
			gui.set_text(clones["text_pid"], product.productIdentifier)
			gui.set_text(clones["text_title"], product.localizedTitle)
			gui.set_text(clones["text_desc"], product.localizedDescription)
			gui.set_text(clones["text_price_locale"], product.priceLocale)
			gui.set_text(clones["text_price"], product.price)
			item:addContent(clones[rootName])
			local size = gui.get_size(clones[rootName])
			item:setItemSize(size.x, size.y)
			item.m_productId = product.productIdentifier
			self.m_listProducts:addItem(item)
		end
	end
	self.m_listProducts:reload()
end

function test22:makePurchase(productId)
	ccprint("buy %s", productId)
	local ok, res = appleiap.purchase({
		productId = productId,
	})
	gwjui.printf("make purchase,ok=%s,res=%s", tostring(ok), tostring(res))
end

function test22:onTransactionCallback(transaction)
	gwjui.dump(transaction, "transaction callback")
	appleiap.finishTransaction({
		transactionId = transaction.transactionIdentifier,
	})
end

return test22
