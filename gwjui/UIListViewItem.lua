local gwjui = require("gwjui.gwjui_impl")

local clsUIListViewItem = gwjui.class("UIListViewItem")

function clsUIListViewItem:ctor(params)
	params = params or {}
	for k,v in pairs(params) do
		self[k] = v
	end
	local node = gui.new_box_node(vmath.vector3(), vmath.vector3())
	local test = false
	if(test) then
		gui.set_color(node, colors[idxColor])
		idxColor = math.modf((idxColor-1+1) % (#colors)) + 1
	else
		gui.set_color(node, vmath.vector4(0, 0, 0, 1))
		gui.set_blend_mode(node, gui.BLEND_ADD)
	end
	self.m_node = node
	self.m_width = 0
	self.m_height = 0
	self.m_onSizeChange = nil
end

function clsUIListViewItem:onSizeChange(func)
	self.m_onSizeChange = func
end

function clsUIListViewItem:setItemSize(w, h, triggerCallback)
	if(triggerCallback == nil) then triggerCallback = true end
	local sizeOld = gwjui.size(self.m_width, self.m_height)
	local sizeNew = gwjui.clone(sizeOld)
	if(w) then
		sizeNew.width = w
	end
	if(h) then
		sizeNew.height = h
	end
	if(sizeNew.width == sizeOld.width and sizeNew.height == sizeOld.height) then
		return
	end
	self.m_width = sizeNew.width
	self.m_height = sizeNew.height
	gui.set_size(self.m_node, vmath.vector3(sizeNew.width, sizeNew.height, 0))

	if(triggerCallback and self.m_onSizeChange) then
		self.m_onSizeChange(self, sizeOld, sizeNew)
	end
end

function clsUIListViewItem:getItemSize()
	return self.m_width, self.m_height
end

function clsUIListViewItem:getNode()
	return self.m_node
end

function clsUIListViewItem:addContent(contentNode)
	gui.set_parent(contentNode, self.m_node)
	gui.set_position(contentNode, vmath.vector3())
	self.m_contentNode = contentNode
end

function clsUIListViewItem:getContent()
	return self.m_contentNode
end

return clsUIListViewItem
