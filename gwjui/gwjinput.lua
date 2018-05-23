--管理所有UI组件输入的模块
local gwjui = require("gwjui.gwjui_impl")

local gwjinput = gwjui.class("gwjinput")

gwjinput.s_recentlyInstance = nil
gwjinput.s_inputCaptureKB = nil

--创建一个输入实例
function gwjinput.createInstance(...)
	local instance = gwjinput.new(...)
	gwjinput.s_recentlyInstance = instance
	return instance
end

function gwjinput.setCaptureKeyboard(obj)
	local old = gwjinput.s_inputCaptureKB
	if(obj ~= gwjinput.s_inputCaptureKB) then
		if(gwjinput.s_inputCaptureKB) then
			gwjinput.s_inputCaptureKB:onReleaseKBCapture()
		end
		gwjinput.s_inputCaptureKB = obj
	end
	return old
end

--在gui_script的final中调用此函数
function gwjinput:final()
	self.m_allObjects = {}
	self.m_objectCaptured = nil
	self.m_allUpdateFunc = {}
	gwjinput.s_recentlyInstance = nil
	gwjinput.s_inputCaptureKB = nil
end

--往input中插入一个对象
--InputObject对象
function gwjinput:addObject(obj)
	for k,v in pairs(self.m_allObjects) do
		if(v == obj) then
			return
		end
	end
	table.insert(self.m_allObjects, obj)
end

--在on_input中调用
function gwjinput:onInput(action_id, action)
	if(action_id == self.m_actionIdTouch) then
		self:handleTouchEvent_(action)
	elseif(action_id == self.m_actionIdText) then
		self:handleTextEvent_(action)
	elseif(action_id == self.m_actionIdBackspace) then
		self:handleBackSpace_(action)
	elseif(action_id == self.m_actionIdMarkedtext) then
	end
end

--当收到on_input的touch事件时调用
function gwjinput:handleTouchEvent_(action)
	if(action.pressed) then--began
		self.m_xDown = action.x
		self.m_yDown = action.y
--		gwjui.printf("began:objectnum:%d", #self.m_allObjects)
		--判断点中了哪个node
		self.m_objectCaptured = nil
		for i,obj in ipairs(self.m_allObjects) do
			local node = obj:getMainNode()
			local visible = obj:isVisible()
			local touchable = obj:isTouchEnabled()
			if(node and visible and touchable and gui.pick_node(node, action.x, action.y)) then
				self.m_objectCaptured = obj
				break
			end
		end
		if(self.m_objectCaptured) then
			self.m_lastTouchPt = gwjui.point(action.x, action.y)
			self.m_objectCaptured:onTouchEvent({
				name = "began",
				x = action.x,
				y = action.y,
				prevX = self.m_lastTouchPt.x,
				prevY = self.m_lastTouchPt.y,
			})
		else
			gwjinput.setCaptureKeyboard(nil)
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

function gwjinput:handleTextEvent_(action)
	if(gwjinput.s_inputCaptureKB == nil) then
		return
	end
	gwjinput.s_inputCaptureKB:onTextEvent(action)
end

function gwjinput:handleBackSpace_(action)
	if(gwjinput.s_inputCaptureKB == nil) then
		return
	end
	gwjinput.s_inputCaptureKB:onBackspace(action)
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

function gwjinput:ctor(params)
	params = params or {}
	self.m_actionIdTouch = hash(params.actionIdTouch or "touch")
	self.m_actionIdText = hash(params.actionIdText or "text")
	self.m_actionIdBackspace = hash(params.actionIdBackspace or "backspace")
	self.m_actionIdMarkedtext = hash(params.actionIdMarkedtext or "marked-text")
	self.m_allObjects = {}
	self.m_objectCaptured = nil
	self.m_lastTouchPt = gwjui.point(0, 0)
	self.m_allUpdateFunc = {}
end

return gwjinput
