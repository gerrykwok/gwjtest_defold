--管理所有UI组件输入的模块
local gwjui = require("gwjui.gwjui_impl")

local gwjinput = gwjui.class("gwjinput")

gwjinput.s_recentlyInstance = nil
gwjinput.s_inputCaptureKB = nil
gwjinput.s_objSwallowTouch = nil
gwjinput.s_allInput = {}

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
			if(tostring(msg.url()) ~= gwjinput.s_inputCaptureKB:getUrl()) then
				return old
			end
			gwjinput.s_inputCaptureKB:onReleaseKBCapture()
		end
		gwjinput.s_inputCaptureKB = obj
	end
	return old
end

function gwjinput.getCurrentInput()
	local url = msg.url()
	local input = gwjinput.s_allInput[tostring(url)]
	return input
end

function gwjinput:ctor(params)
	params = params or {}
	self.m_actionIdTouch = hash(params.actionIdTouch or "touch")
	self.m_actionIdText = hash(params.actionIdText or "text")
	self.m_actionIdBackspace = hash(params.actionIdBackspace or "backspace")
	self.m_actionIdMarkedtext = hash(params.actionIdMarkedtext or "marked-text")
	self.m_allObjects = {}
	self.m_allObjCaptured = {}
	gwjinput.s_objSwallowTouch = nil
	self.m_lastTouchPt = gwjui.point(0, 0)
	self.m_allUpdateFunc = {}
	self.m_name = ""
	self.m_url = tostring(msg.url())
	gwjinput.s_allInput[self.m_url] = self
end

--在gui_script的final中调用此函数
function gwjinput:final()
	local allObj = self.m_allObjects
	self.m_allObjects = {}
	for k,obj in pairs(allObj) do
		if(obj.onExit) then
			obj:onExit()
		end
	end
	self.m_allObjCaptured = {}
	gwjinput.s_objSwallowTouch = nil
	self.m_allUpdateFunc = {}
	gwjinput.s_recentlyInstance = nil
	gwjinput.s_inputCaptureKB = nil
	gwjinput.s_allInput[self.m_url] = nil
	gwjui.TextureCache.getInstance():final()
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

--从input中去掉对象
function gwjinput:removeObject(obj)
	for i=#self.m_allObjects,1,-1 do
		if(self.m_allObjects[i] == obj) then
			table.remove(self.m_allObjects, i)
		end
	end
end

--在on_input中调用
function gwjinput:onInput(action_id, action)
	if(action_id == self.m_actionIdTouch) then
--		gwjui.printf("touch,url=%s", tostring(self.m_url))
		self:handleTouchEvent_(action)
	elseif(action_id == self.m_actionIdText) then
		self:handleTextEvent_(action)
	elseif(action_id == self.m_actionIdBackspace) then
		self:handleBackSpace_(action)
	elseif(action_id == self.m_actionIdMarkedtext) then
	end
end

--在on_message中调用
function gwjinput:on_message(message_id, message, sender)
end

--如果节点被父节点clipped，应该还要判断父节点
function gwjinput.pickNode(node, x, y)
	if(not gui.pick_node(node, x, y)) then
		return false
	end
	node = gui.get_parent(node)
	while(node) do
		local isPick = gui.pick_node(node, x, y)
		local clippingMode = gui.get_clipping_mode(node)
		if(clippingMode == gui.CLIPPING_MODE_STENCIL and not isPick) then
			return false
		end

		node = gui.get_parent(node)		
	end
	return true
end

--当收到on_input的touch事件时调用
function gwjinput:handleTouchEvent_(action)
	if(action.pressed) then--began
		if(gwjinput.s_objSwallowTouch) then--当前有节点吞噬了触摸
			return
		end
		self.m_xDown = action.x
		self.m_yDown = action.y
		local debug = false
		if(debug) then
			local str = ""
			for i=#self.m_allObjects,1,-1 do
				local obj = self.m_allObjects[i]
				if(str ~= "") then str = str .. "," end
				str = str .. obj:getName()
			end
			gwjui.printf("began,objects:%s", str)
		end
		--删除无效的对象
		self:deleteInvalidObj()
		--判断点中了哪个node
		self.m_allObjCaptured = {}
		for i=#self.m_allObjects,1,-1 do
			local obj = self.m_allObjects[i]
			local node = obj:getMainNode()

			local flag,enable = pcall(function(node)
				return gui.is_enabled(node)
			end, obj:getMainNode())

			if(flag) then
				local visible = obj:isVisibleInScreen()
				local touchable = obj:isTouchEnabled()
				if(node and visible and touchable and gwjinput.pickNode(node, action.x, action.y)) then
					table.insert(self.m_allObjCaptured, obj)
					if(obj:isTouchSwallowEnabled()) then--该节点吞噬了触摸
						gwjinput.s_objSwallowTouch = obj
						break
					end
				end
			else
				gwjui.printf("err:%s", enable)
			end
		end
		--gwjinput.setCaptureKeyboard(self.m_objectCaptured)
		self.m_lastTouchPt = gwjui.point(action.x, action.y)
		for i,obj in ipairs(self.m_allObjCaptured) do
			obj:onTouchEvent({
				name = "began",
				x = action.x,
				y = action.y,
				prevX = self.m_lastTouchPt.x,
				prevY = self.m_lastTouchPt.y,
			})
		end

		if(#self.m_allObjCaptured <= 0) then
			gwjinput.setCaptureKeyboard(nil)
		else
			local found = false
			for k,v in pairs(self.m_allObjCaptured) do
				if(v == gwjinput.s_inputCaptureKB) then
					found = true
					break
				end
			end
			if(not found) then
				gwjinput.setCaptureKeyboard(nil)
			end
		end
	elseif(action.released) then--ended
--		gwjui.printf("ended:%f,%f", action.x, action.y)
		for i,obj in ipairs(self.m_allObjCaptured) do
			obj:onTouchEvent({
				name = "ended",
				x = action.x,
				y = action.y,
				prevX = self.m_lastTouchPt.x,
				prevY = self.m_lastTouchPt.y,
			})
		end
		self.m_allObjCaptured = {}
		gwjinput.s_objSwallowTouch = nil
	else--moved
		if(action.x ~= self.m_xDown or action.y ~= self.m_yDown) then
--			gwjui.printf("moved:%f,%f", action.x, action.y)
			self.m_xDown = action.x
			self.m_yDown = action.y
			for i,obj in ipairs(self.m_allObjCaptured) do
				obj:onTouchEvent({
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
	local objCapture = gwjinput.s_inputCaptureKB
	if(objCapture == nil) then
		return
	end
	objCapture:onTextEvent(action)
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

function gwjinput:setName(name)
	self.m_name = name
end

--删除已经无效的对象
function gwjinput:deleteInvalidObj()
	for i=#self.m_allObjects,1,-1 do
		local obj = self.m_allObjects[i]
		local flag,enable = pcall(function(node)
			return gui.is_enabled(node)
		end, obj:getMainNode())
		if(not flag) then
			table.remove(self.m_allObjects, i)
		end
	end
end

return gwjinput
