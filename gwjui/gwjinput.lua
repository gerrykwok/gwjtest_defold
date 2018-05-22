--管理所有UI组件输入的模块
local gwjui = require("gwjui.gwjui_impl")
local gwjinput = gwjui.class("gwjinput")

gwjinput.s_recentlyInstance = nil

--创建一个输入实例
function gwjinput.createInstance(...)
	local instance = gwjinput.new(...)
	gwjinput.s_recentlyInstance = instance
	return instance
end

--在gui_script的final中调用此函数
function gwjinput:final()
	self.m_allObjects = {}
	self.m_objectCaptured = nil
	self.m_allUpdateFunc = {}
end

--往input中插入一个对象
--GwjInputObject对象
function gwjinput:addObject(obj)
	for k,v in pairs(self.m_allObjects) do
		if(v == obj) then
			return
		end
	end
	table.insert(self.m_allObjects, obj)
end

--当收到on_input的touch事件时调用
function gwjinput:handleTouchEvent(action)
	if(action.pressed) then--began
		self.m_xDown = action.x
		self.m_yDown = action.y
--		gwjui.printf("began:objectnum:%d", #self.m_allObjects)
		--判断点中了哪个node
		for i,obj in ipairs(self.m_allObjects) do
			local node = obj:getMainNode()
			local visible = obj:isVisible()
			if(node and visible and gui.pick_node(node, action.x, action.y)) then
				self.m_objectCaptured = obj
				self.m_lastTouchPt = gwjui.point(action.x, action.y)
				obj:onTouchEvent({
					name = "began",
					x = action.x,
					y = action.y,
					prevX = self.m_lastTouchPt.x,
					prevY = self.m_lastTouchPt.y,
				})
				break
			end
		end
	elseif(action.released) then--ended
--		gwjui.printf("ended:%f,%f", action.x, action.y)
		if(self.m_objectCaptured) then
			self.m_objectCaptured:onTouchEvent({
				name = "ended",
				x = action.x,
				y = action.y,
				prevX = self.m_lastTouchPt.x,
				prevY = self.m_lastTouchPt.y,
			})
		end
		self.m_objectCaptured = nil
	else--moved
		if(action.x ~= self.m_xDown or action.y ~= self.m_yDown) then
--			gwjui.printf("moved:%f,%f", action.x, action.y)
			self.m_xDown = action.x
			self.m_yDown = action.y
			if(self.m_objectCaptured) then
				self.m_objectCaptured:onTouchEvent({
					name = "moved",
					x = action.x,
					y = action.y,
					prevX = self.m_lastTouchPt.x,
					prevY = self.m_lastTouchPt.y,
				})
			end
			self.m_lastTouchPt.x = action.x
			self.m_lastTouchPt.y = action.y
		end
	end
end

--在update中调用
function gwjinput:onUpdate(dt)
	for i,func in ipairs(self.m_allUpdateFunc) do
		func(dt)
	end
end

function gwjinput:scheduleUpdate(func)
	local found = false
	for i,f in ipairs(self.m_allUpdateFunc) do
		if(f == func) then
			found = true
			break
		end
	end
	if(not found) then
		table.insert(self.m_allUpdateFunc, func)
	end
end

function gwjinput:unscheduleUpdate(func)
	for i=#self.m_allUpdateFunc,1,-1 do
		if(self.m_allUpdateFunc[i] == func) then
			table.remove(self.m_allUpdateFunc, i)
		end
	end
end

--------------------------------------------------------------------------------------------------------------------
--华丽的分隔线

function gwjinput:ctor()
	self.m_allObjects = {}
	self.m_objectCaptured = nil
	self.m_lastTouchPt = gwjui.point(0, 0)
	self.m_allUpdateFunc = {}
end

return gwjinput
