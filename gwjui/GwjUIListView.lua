--------------------------------------------------------------------
-- Author: guoweiji
-- Start Date: 2018-05-14
-- Description: 模仿cocos2dx的listview

local gwjui = require("gwjui.gwjui")

local clsGwjUIListView = gwjui.class("GwjUIListView")

clsGwjUIListView.DIRECTION_VERTICAL = 1
clsGwjUIListView.DIRECTION_HORIZONTAL = 2
clsGwjUIListView.s_allListView = {}
clsGwjUIListView.s_listviewDown = nil
clsGwjUIListView.DEFAULT_SHAKE_DISTANCE = 8
clsGwjUIListView.s_lastTouchXY = {}

-- Convert string to hash, unless it's already a hash
-- @param str String to convert
-- @return The hashed string
function clsGwjUIListView.to_hash(str)
	return type(str) == "string" and hash(str) or str
end

function clsGwjUIListView.createInstance(...)
	return clsGwjUIListView.new(...)
end

function clsGwjUIListView.handleTouchEvent(action)
	if(action.pressed) then--began
		clsGwjUIListView.m_xDown = action.x
		clsGwjUIListView.m_yDown = action.y
--		gwjui.printf("began:%f,%f", action.x, action.y)
		--判断点中了哪个listview
		for i,listview in ipairs(clsGwjUIListView.s_allListView) do
			if(gui.pick_node(listview.m_listNode, action.x, action.y)) then
				clsGwjUIListView.s_listviewDown = listview
				clsGwjUIListView.s_lastTouchXY = gwjui.point(action.x, action.y)
				listview:onTouch_({
					name = "began",
					x = action.x,
					y = action.y,
					prevX = clsGwjUIListView.s_lastTouchXY.x,
					prevY = clsGwjUIListView.s_lastTouchXY.y,
				})
				break
			end
		end
	elseif(action.released) then--ended
--		gwjui.printf("ended:%f,%f", action.x, action.y)
		if(clsGwjUIListView.s_listviewDown) then
			clsGwjUIListView.s_listviewDown:onTouch_({
				name = "ended",
				x = action.x,
				y = action.y,
				prevX = clsGwjUIListView.s_lastTouchXY.x,
				prevY = clsGwjUIListView.s_lastTouchXY.y,
			})
		end
		clsGwjUIListView.s_listviewDown = nil
	else--moved
		if(action.x ~= clsGwjUIListView.m_xDown or action.y ~= clsGwjUIListView.m_yDown) then
--			gwjui.printf("moved:%f,%f", action.x, action.y)
			clsGwjUIListView.m_xDown = action.x
			clsGwjUIListView.m_yDown = action.y
			if(clsGwjUIListView.s_listviewDown) then
				clsGwjUIListView.s_listviewDown:onTouch_({
					name = "moved",
					x = action.x,
					y = action.y,
					prevX = clsGwjUIListView.s_lastTouchXY.x,
					prevY = clsGwjUIListView.s_lastTouchXY.y,
				})
			end
			clsGwjUIListView.s_lastTouchXY.x = action.x
			clsGwjUIListView.s_lastTouchXY.y = action.y
		end
	end
end

function clsGwjUIListView.final()
	clsGwjUIListView.s_listviewDown = nil
	clsGwjUIListView.s_allListView = {}
end

function clsGwjUIListView:ctor(params)
	table.insert(clsGwjUIListView.s_allListView, self)
	params = params or {}
	local list_id = params.list_id
	self.m_listNode = gui.get_node(list_id)
	self.m_allItems = {}
	self.m_direction = params.direction or clsGwjUIListView.DIRECTION_VERTICAL
	self.m_nShake = clsGwjUIListView.DEFAULT_SHAKE_DISTANCE
	self.m_scrollContentSize = gwjui.size(0, 0)
	local size = gui.get_size(self.m_listNode)
	self.m_viewSize = gwjui.size(size.x, size.y)
--	gwjui.dump(size, "node size")
	local anchor = gwjui.getGuiAnchorPoint(self.m_listNode)
--	gwjui.printf("gwjgwj,anchor:%f,%f", anchor.x, anchor.y)
	--创建scrollnode
	local node = gui.new_box_node(vmath.vector3(-anchor.x*size.x, -anchor.y*size.y, 0), vmath.vector3())
	gui.set_parent(node, self.m_listNode)
	self.m_scrollNode = gui.new_box_node(vmath.vector3(0, size.y, 0), vmath.vector3())
	gui.set_parent(self.m_scrollNode, node)
end

function clsGwjUIListView:isVertical()
	return self.m_direction == clsGwjUIListView.DIRECTION_VERTICAL
end

local colors = {
	vmath.vector4(1, 1, 1, 1),
	vmath.vector4(1, 0, 0, 1),
	vmath.vector4(0, 1, 0, 1),
	vmath.vector4(0, 0, 1, 1),
	vmath.vector4(1, 1, 0, 1),
}
local idxColor = 1
function clsGwjUIListView:addItem(item_template_id, itemWidth, itemHeight)
	item_template_id = clsGwjUIListView.to_hash(item_template_id)
	local clones = gui.clone_tree(gui.get_node(item_template_id))

	local root = clones[item_template_id]
	local wh = gui.get_size(root)
	itemWidth = itemWidth or wh.x
	itemHeight = itemHeight or wh.y

	local wh = {}
	if(self:isVertical()) then
		wh.width = self.m_viewSize.width
		wh.height = itemHeight
	else
		wh.width = itemWidth
		wh.height = self.m_viewSize.height
	end
	local node = gui.new_box_node(vmath.vector3(), vmath.vector3(wh.width, wh.height, 0))
	gui.set_parent(node, self.m_scrollNode)
	gui.set_position(root, vmath.vector3())
	gui.set_parent(root, node)
	local test = false
	if(test) then
		gui.set_color(node, colors[idxColor])
		idxColor = math.modf((idxColor-1+1) % (#colors)) + 1
	else
		gui.set_color(node, vmath.vector4(0, 0, 0, 1))
		gui.set_blend_mode(node, gui.BLEND_ADD)
	end
	local item = {
		node = node,
		idx = #self.m_allItems + 1,
		width = itemWidth,
		height = itemHeight,
	}
	table.insert(self.m_allItems, item)
	return item
end

function clsGwjUIListView:reload()
	local viewSize = self.m_viewSize
	--计算内容的总高度或总宽度
	if(self.m_direction == clsGwjUIListView.DIRECTION_VERTICAL) then
		self.m_scrollContentSize.width = viewSize.width
		local totalHeight = 0
		for i,item in ipairs(self.m_allItems) do
			totalHeight = totalHeight + item.height
		end
		self.m_scrollContentSize.height = totalHeight
	else
		self.m_scrollContentSize.height = viewSize.height
		local totalWidth = 0
		for i,item in ipairs(self.m_allItems) do
			totalWidth = totalWidth + item.width
		end
		self.m_scrollContentSize.width = totalWidth
	end
	print("scrollContentSize:", self.m_scrollContentSize.width, self.m_scrollContentSize.height)
	print("viewSize:", viewSize.width, viewSize.height)
	--设定每个item的位置
	if(self.m_direction == clsGwjUIListView.DIRECTION_VERTICAL) then
		local x = viewSize.width / 2
		local y = self.m_scrollContentSize.height
		for i,item in ipairs(self.m_allItems) do
			gui.set_position(item.node, vmath.vector3(x, y-item.height/2, 0))
			y = y - item.height
		end
	else
		local x = 0
		local y = viewSize.height / 2
		for i,item in ipairs(self.m_allItems) do
			gui.set_position(item.node, vmath.vector3(x+item.width/2, y, 0))
			x = x + item.width
		end
	end
	gui.set_position(self.m_scrollNode, vmath.vector3(0, viewSize.height-self.m_scrollContentSize.height, 0))
end

function clsGwjUIListView:onTouch(listener)
	self.m_onTouch = listener
end

function clsGwjUIListView:notifyListener_(event)
	if not self.m_onTouch then
		return
	end
	self.m_onTouch(event)
end

function clsGwjUIListView:onTouch_(event)
--	gwjui.printf("gwjgwj,event:%s", event.name)
	if(event.name == "began") then
		self.m_prevXY = gwjui.point(event.x, event.y)
		self.m_posScroll = gui.get_position(self.m_scrollNode)
		self.m_bDrag = false
		self:notifyListener_({name="began", x=event.x, y=event.y})
	elseif(event.name == "ended") then
		if(self.m_bDrag) then
			self:notifyListener_({name="ended", x=event.x, y=event.y})
		else
			--判断点中了哪个item
			local item = self:hitTest(event)
			if(item) then
				self:notifyListener_({name="clicked",
					listView = self, itemPos = item.idx, item = item,
					x=event.x, y=event.y})
			end
		end
	elseif(event.name == "moved") then
		if(self:isShake_(event)) then
			return
		end
		self.m_bDrag = true
		local pos = gui.get_position(self.m_scrollNode)
		local x = pos.x + event.x - event.prevX
		local y = pos.y + event.y - event.prevY
		
		local viewSize = self.m_viewSize
		if(self.m_direction == clsGwjUIListView.DIRECTION_VERTICAL) then
			if(y+self.m_scrollContentSize.height < viewSize.height) then
				y = viewSize.height - self.m_scrollContentSize.height
			end
			if(self.m_scrollContentSize.height > viewSize.height) then
				if(y > 0) then
					y = 0
				end
			else
				y = viewSize.height - self.m_scrollContentSize.height
			end
			gwjui.setGuiPos(self.m_scrollNode, {y=y})
		else
			if(x > 0) then x = 0 end
			if(self.m_scrollContentSize.width > viewSize.width) then
				if(x+self.m_scrollContentSize.width < viewSize.width) then
					x = viewSize.width-self.m_scrollContentSize.width
				end
			end
			gwjui.setGuiPos(self.m_scrollNode, {x=x})
		end
		self:notifyListener_({name="moved", x=event.x, y=event.y})
	end
end

function clsGwjUIListView:isShake_(event)
	if(self.m_direction == clsGwjUIListView.DIRECTION_VERTICAL) then
		return math.abs(event.y - self.m_prevXY.y) < self.m_nShake
	else
		return math.abs(event.x - self.m_prevXY.x) < self.m_nShake
	end
end

function clsGwjUIListView:hitTest(event)
	for i,item in ipairs(self.m_allItems) do
		if(gui.pick_node(item.node, event.x, event.y)) then
			return item
		end
	end
end

return clsGwjUIListView
