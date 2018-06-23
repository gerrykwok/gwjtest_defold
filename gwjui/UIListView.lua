--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-14
-- Description: 模仿cocos2dx的listview

local gwjui = require("gwjui.gwjui_impl")
local gwjinput = require("gwjui.gwjinput")
local InputObject = require("gwjui.InputObject")
local UIListViewItem = require("gwjui.UIListViewItem")

local clsUIListView = gwjui.class("UIListView", InputObject)
if false then
	UIListView = clsUIListView
end

clsUIListView.DIRECTION_VERTICAL = 1
clsUIListView.DIRECTION_HORIZONTAL = 2
clsUIListView.DEFAULT_SHAKE_DISTANCE = 5

--for debug only
local colors = {
	vmath.vector4(1, 1, 1, 1),
	vmath.vector4(1, 0, 0, 1),
	vmath.vector4(0, 1, 0, 1),
	vmath.vector4(0, 0, 1, 1),
	vmath.vector4(1, 1, 0, 1),
}
local idxColor = 1

function clsUIListView:ctor(params)
	clsUIListView.super.ctor(self, params)
	gui.set_clipping_mode(self.m_mainNode, gui.CLIPPING_MODE_STENCIL)
	self.m_allItems = {}
	self.m_direction = params.direction or clsUIListView.DIRECTION_VERTICAL
	self.m_autoHideScrollBar = params.autoHideScrollBar
	local scrollbarTrackId = params.scrollbarTrackId
	local scrollbarThumbId = params.scrollbarThumbId
	self.m_nShake = clsUIListView.DEFAULT_SHAKE_DISTANCE
	self.m_scrollContentSize = gwjui.size(0, 0)
	local size = gui.get_size(self.m_mainNode)
	self.m_viewSize = gwjui.size(size.x, size.y)
--	gwjui.dump(size, "node size")
	local anchor = gwjui.getGuiAnchorPoint(self.m_mainNode)
--	gwjui.printf("gwjgwj,anchor:%f,%f", anchor.x, anchor.y)
	--创建scrollnode
	local node = gui.new_box_node(vmath.vector3(-anchor.x*size.x, -anchor.y*size.y, 0), vmath.vector3())
	gui.set_parent(node, self.m_mainNode)
	self.m_scrollNode = gui.new_box_node(vmath.vector3(), vmath.vector3())
	gui.set_parent(self.m_scrollNode, node)
	--滚动条
	if(scrollbarTrackId and scrollbarThumbId) then
		self.m_scrollbarTrack = gui.get_node(scrollbarTrackId)
		self.m_scrollbarThumb = gui.get_node(scrollbarThumbId)
		gui.set_parent(self.m_scrollbarTrack, self.m_mainNode)
		gui.set_parent(self.m_scrollbarThumb, self.m_scrollbarTrack)

		local sizeTrack = gui.get_size(self.m_scrollbarTrack)
		local sizeThumb = gui.get_size(self.m_scrollbarThumb)

		local size = gui.get_size(self.m_scrollbarTrack)
		size.y = self.m_viewSize.height
		gui.set_size(self.m_scrollbarTrack, size)
		gui.set_pivot(self.m_scrollbarTrack, gui.PIVOT_S)
		gui.set_position(self.m_scrollbarTrack, vmath.vector3((1-anchor.x)*self.m_viewSize.width-math.max(sizeTrack.x, sizeThumb.x)/2, -anchor.y*self.m_viewSize.height, 0))

		gui.set_position(self.m_scrollbarThumb, vmath.vector3(0, self.m_viewSize.height/2, 0))
	end

	self.m_speed = {x=0,y=0}
	self.m_bBounce = true

	self:setTouchEnabled(true)
	self:setTouchEventListener(gwjui.handler(self, self.onTouch_))
	self.m_updateFunc = gwjui.handler(self, self.update_)
	gwjui.scheduleUpdate(self.m_updateFunc)
end

function clsUIListView:setBounceable(bBounceable)
	self.m_bBounce = bBounceable
end

function clsUIListView:isVertical()
	return self.m_direction == clsUIListView.DIRECTION_VERTICAL
end

function clsUIListView:newItem()
	local item = UIListViewItem.new()
	item:onSizeChange(gwjui.handler(self, self.itemSizeChangeListener))
	return item
end

function clsUIListView:addItem(listItem, pos)
	self:modifyItemSizeIf_(listItem)
	if pos then
		table.insert(self.m_allItems, pos, listItem)
	else
		table.insert(self.m_allItems, listItem)
	end
	gui.set_parent(listItem:getNode(), self.m_scrollNode)
end

function clsUIListView:modifyItemSizeIf_(listItem)
	local w, h = listItem:getItemSize()

	if self:isVertical() then
		if w ~= self.m_viewSize.width then
			listItem:setItemSize(self.m_viewSize.width, h)
		end
	else
		if h ~= self.m_viewSize.height then
			listItem:setItemSize(w, self.m_viewSize.height)
		end
	end
end

function clsUIListView:itemSizeChangeListener(listItem, sizeOld, sizeNew)
	local itemPos = self:getItemPos(listItem)
	if(itemPos == nil) then
		return
	end
	local itemW, itemH = sizeNew.width - sizeOld.width, sizeNew.height - sizeOld.height
	if(self.m_direction == clsUIListView.DIRECTION_VERTICAL) then
		itemW = 0
	else
		itemH = 0
	end
	self.m_scrollContentSize.width = self.m_scrollContentSize.width + itemW
	self.m_scrollContentSize.height = self.m_scrollContentSize.height + itemH
	self:moveItems(itemPos, itemPos, itemW/2, itemH/2, false)
	if(self.m_direction == clsUIListView.DIRECTION_VERTICAL) then
		gwjui.moveNode(self.m_scrollNode, -itemW, -itemH)
		self:moveItems(1, itemPos-1, itemW, itemH, false)
	else
		self:moveItems(itemPos+1, #self.m_allItems, itemW, itemH, false)
	end
end

function clsUIListView:getItemPos(listItem)
	for i,v in ipairs(self.m_allItems) do
		if(v == listItem) then
			return i
		end
	end
end

function clsUIListView:moveItems(beginIdx, endIdx, x, y, bAni)
	if(beginIdx == 1 and endIdx == 0) then
		self:elasticScroll();
		return;
	end

	local posX, posY = 0, 0

	local moveByParams = {x = x, y = y, time = 0.2}
	for i=beginIdx, endIdx do
		local node = self.m_allItems[i]:getNode()
		local pos = gui.get_position(node)
		if bAni then
			if i == endIdx then
				moveByParams.onComplete = function()
					self:elasticScroll()
				end
			else
				moveByParams.onComplete = nil
			end
			pos.x = pos.x + moveByParams.x
			pos.y = pos.y + moveByParams.y
			gui.animate(node, "position", pos, gui.EASING_LINEAR, moveByParams.time, 0, moveByParams.onComplete)
		else
			pos.x = pos.x + moveByParams.x
			pos.y = pos.y + moveByParams.y
			gui.set_position(node, pos)
			if i == endIdx then
				self:elasticScroll()
			end
		end
	end
end

function clsUIListView:removeAllItems()
	for i,item in ipairs(self.m_allItems) do
		gui.delete_node(item:getNode())
	end
	self.m_allItems = {}
end

function clsUIListView:reload()
	local viewSize = self.m_viewSize
	--计算内容的总高度或总宽度
	if(self.m_direction == clsUIListView.DIRECTION_VERTICAL) then
		self.m_scrollContentSize.width = viewSize.width
		local totalHeight = 0
		for i,item in ipairs(self.m_allItems) do
			local w,h = item:getItemSize()
			totalHeight = totalHeight + h
		end
		self.m_scrollContentSize.height = totalHeight
	else
		self.m_scrollContentSize.height = viewSize.height
		local totalWidth = 0
		for i,item in ipairs(self.m_allItems) do
			local w,h = item:getItemSize()
			totalWidth = totalWidth + w
		end
		self.m_scrollContentSize.width = totalWidth
	end
--	print("scrollContentSize:", self.m_scrollContentSize.width, self.m_scrollContentSize.height)
--	print("viewSize:", viewSize.width, viewSize.height)
	--设定每个item的位置
	if(self.m_direction == clsUIListView.DIRECTION_VERTICAL) then
		local x = viewSize.width / 2
		local y = self.m_scrollContentSize.height
		for i,item in ipairs(self.m_allItems) do
			local w,h = item:getItemSize()
			gui.set_position(item:getNode(), vmath.vector3(x, y-h/2, 0))
			y = y - h
		end
	else
		local x = 0
		local y = viewSize.height / 2
		for i,item in ipairs(self.m_allItems) do
			local w,h = item:getItemSize()
			gui.set_position(item:getNode(), vmath.vector3(x+w/2, y, 0))
			x = x + w
		end
	end
	gui.set_position(self.m_scrollNode, vmath.vector3(0, viewSize.height-self.m_scrollContentSize.height, 0))
	--滚动条的长度
	if(self.m_scrollbarThumb) then
		local bound = self:getScrollNodeRect()
		local barH = self.m_viewSize.height*self.m_viewSize.height/bound.height
		local size = gui.get_size(self.m_scrollbarThumb)
		if barH < size.x then-- 保证bar不会太小
			barH = size.x
		end
		if(barH > self.m_viewSize.height) then--保证bar不会太大
			barH = self.m_viewSize.height
		end
		size.y = barH
		gui.set_size(self.m_scrollbarThumb, size)
	end
end

function clsUIListView:onTouch(listener)
	self.m_onTouch = listener
	return self
end

function clsUIListView:notifyListener_(event)
	if not self.m_onTouch then
		return
	end
	self.m_onTouch(event)
end

function clsUIListView:onTouch_(event)
--	gwjui.printf("gwjgwj,event:%s", event.name)
	if(event.name == "began") then
		self.m_prevXY = gwjui.point(event.x, event.y)
		self.m_bDrag = false
		self:enableScrollBar()
		gui.cancel_animation(self.m_scrollNode, "position")
		self:notifyListener_({name="began", x=event.x, y=event.y})
	elseif(event.name == "moved") then
		if(self:isShake_(event)) then
			return
		end
		self.m_bDrag = true
		self.m_speed.x = event.x - event.prevX
		self.m_speed.y = event.y - event.prevY
		if(self.m_direction == clsUIListView.DIRECTION_VERTICAL) then
			self.m_speed.x = 0
		elseif(self.m_direction == clsUIListView.DIRECTION_HORIZONTAL) then
			self.m_speed.y = 0
		end
		self:scrollBy(self.m_speed.x, self.m_speed.y)
		self:notifyListener_({name="moved", x=event.x, y=event.y})
	elseif(event.name == "ended") then
		if(self.m_bDrag) then
			self.m_bDrag = false
			self:scrollAuto()
			self:disableScrollBar()
			self:notifyListener_({name="ended", x=event.x, y=event.y})
		else
			--判断点中了哪个item
			local item,idx = self:hitTest(event)
			if(item) then
				self:notifyListener_({name="clicked",
					listView = self, itemPos = idx, item = item,
					x=event.x, y=event.y})
			end
		end
	end
end

function clsUIListView:scrollBy(x, y)
	local pos = gui.get_position(self.m_scrollNode)
	pos.x,pos.y = self:moveXY(pos.x, pos.y, x, y)
	gui.set_position(self.m_scrollNode, pos)
end

function clsUIListView:scrollTo(x, y)
	gui.set_position(self.m_scrollNode, vmath.vector3(x, y, 0))
end

function clsUIListView:scrollAuto()
	if self:twiningScroll() then
		return
	end
	self:elasticScroll()
end

function clsUIListView:twiningScroll()
	if self:isSideShow() then
		-- printInfo("UIScrollView - side is show, so elastic scroll")
		return false
	end

	if math.abs(self.m_speed.x) < 10 and math.abs(self.m_speed.y) < 10 then
		-- printInfo("#DEBUG, UIScrollView - isn't twinking scroll:" .. self.m_speed.x .. " " .. self.m_speed.y)
		return false
	end

	local disX, disY = self:moveXY(0, 0, self.m_speed.x*6, self.m_speed.y*6)
	local pos = gui.get_position(self.m_scrollNode)
	gui.animate(self.m_scrollNode, "position", vmath.vector3(pos.x+disX, pos.y+disY, 0), gui.EASING_OUTSINE, 0.3, 0, function()
		self:elasticScroll()
	end)
end

function clsUIListView:elasticScroll()
	local cascadeBound = self:getScrollNodeRect()
	local disX, disY = 0, 0
	local viewRect = self:getViewRectInSelf()

	if cascadeBound.width < viewRect.width then
		disX = viewRect.x - cascadeBound.x
	else
		if cascadeBound.x > viewRect.x then
			disX = viewRect.x - cascadeBound.x
		elseif cascadeBound.x + cascadeBound.width < viewRect.x + viewRect.width then
			disX = viewRect.x + viewRect.width - cascadeBound.x - cascadeBound.width
		end
	end

	if cascadeBound.height < viewRect.height then
		disY = viewRect.y + viewRect.height - cascadeBound.y - cascadeBound.height
	else
		if cascadeBound.y > viewRect.y then
			disY = viewRect.y - cascadeBound.y
		elseif cascadeBound.y + cascadeBound.height < viewRect.y + viewRect.height then
			disY = viewRect.y + viewRect.height - cascadeBound.y - cascadeBound.height
		end
	end

	if 0 == disX and 0 == disY then
		return
	end

	local pos = gui.get_position(self.m_scrollNode)
	gui.animate(self.m_scrollNode, "position", vmath.vector3(pos.x+disX, pos.y+disY, 0), gui.EASING_OUTBACK, 0.3, 0, function()
		self:notifyListener_{name = "scrollEnd"}
	end)
end

function clsUIListView:isShake_(event)
	if(self.m_direction == clsUIListView.DIRECTION_VERTICAL) then
		return math.abs(event.y - self.m_prevXY.y) < self.m_nShake
	else
		return math.abs(event.x - self.m_prevXY.x) < self.m_nShake
	end
end

function clsUIListView:hitTest(event)
	for i,item in ipairs(self.m_allItems) do
		if(gui.pick_node(item:getNode(), event.x, event.y)) then
			return item,i
		end
	end
end

function clsUIListView:moveXY(orgX, orgY, speedX, speedY)
	if(self.m_bBounce) then
		return orgX + speedX, orgY + speedY--bounce
	end

	local scaleToWorldSpace_ = gwjui.point(1, 1)
	local cascadeBound = self:getScrollNodeRect()
	local viewRect = self:getViewRectInSelf()
	local x, y = orgX, orgY
	local disX, disY

	if speedX > 0 then
		if cascadeBound.x < viewRect.x then
			disX = viewRect.x - cascadeBound.x
			disX = disX / scaleToWorldSpace_.x
			x = orgX + math.min(disX, speedX)
		end
	else
		if cascadeBound.x + cascadeBound.width > viewRect.x + viewRect.width then
			disX = viewRect.x + viewRect.width - cascadeBound.x - cascadeBound.width
			disX = disX / scaleToWorldSpace_.x
			x = orgX + math.max(disX, speedX)
		end
	end

	if speedY > 0 then
		if cascadeBound.y < viewRect.y then
			disY = viewRect.y - cascadeBound.y
			disY = disY / scaleToWorldSpace_.y
			y = orgY + math.min(disY, speedY)
		end
	else
		if cascadeBound.y + cascadeBound.height > viewRect.y + viewRect.height then
			disY = viewRect.y + viewRect.height - cascadeBound.y - cascadeBound.height
			disY = disY / scaleToWorldSpace_.y
			y = orgY + math.max(disY, speedY)
		end
	end

	return x, y
end

-- 是否显示到边缘
function clsUIListView:isSideShow()
	local bound = self:getScrollNodeRect()
	local viewRect = self:getViewRectInSelf()
	if(bound.x > viewRect.x
		or bound.x+bound.width < viewRect.x+viewRect.width
		or bound.y > viewRect.y
		or bound.y+bound.height < viewRect.y+viewRect.height) then
		return true
	end
	return false
end

function clsUIListView:getScrollNodeRect()
	local pos = gui.get_position(self.m_scrollNode)
	local cascadeBound = gwjui.rect(pos.x, pos.y, self.m_scrollContentSize.width, self.m_scrollContentSize.height)
	return cascadeBound
end

function clsUIListView:getViewRectInSelf()
	return gwjui.rect(0, 0, self.m_viewSize.width, self.m_viewSize.height)
end

function clsUIListView:getViewRect()
	return self:getViewRectInSelf()
end

function clsUIListView:update_(dt)
	self:drawScrollBar()
end

function clsUIListView:enableScrollBar()
	if(not self.m_autoHideScrollBar) then
		return
	end
	if(self.m_scrollbarTrack) then
		gui.set_color(self.m_scrollbarTrack, vmath.vector4(1, 1, 1, 1))
	end
	if(self.m_scrollbarThumb) then
		gui.set_color(self.m_scrollbarThumb, vmath.vector4(1, 1, 1, 1))
	end
end

function clsUIListView:disableScrollBar()
	if(not self.m_autoHideScrollBar) then
		return
	end
	if(self.m_scrollbarTrack) then
		gui.animate(self.m_scrollbarTrack, "color.w", 0, gui.EASING_LINEAR, 0.3, 0, function()
		end)
		gui.animate(self.m_scrollbarThumb, "color.w", 0, gui.EASING_LINEAR, 0.3)
	end
end

function clsUIListView:drawScrollBar()
--	if not self.m_bDrag then
--		return
--	end
	if(self.m_scrollbarThumb == nil) then
		return
	end

	local bound = self:getScrollNodeRect()
	local size = gui.get_size(self.m_scrollbarThumb)
	size = gwjui.size(size.x, size.y)
	local viewRect = self:getViewRectInSelf()
	local posY = (viewRect.y - bound.y)*(viewRect.height - size.height)/(bound.height - viewRect.height) + viewRect.y + size.height/2
	local pos = gui.get_position(self.m_scrollbarThumb)
	pos.y = posY
	gui.set_position(self.m_scrollbarThumb, pos)
end

function clsUIListView:isContentMoved()
	return self.m_bDrag
end

return clsUIListView
