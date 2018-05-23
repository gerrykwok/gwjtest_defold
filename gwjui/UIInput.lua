local gwjui = require("gwjui.gwjui_impl")
local gwjinput = require("gwjui.gwjinput")
local InputObject = require("gwjui.InputObject")

local utf8_gfind = "([%z\1-\127\194-\244][\128-\191]*)"

local space_width = {}
-- calculate space width with font
local function get_space_width(font)
	if not space_width[font] then
		local no_space = gui.get_text_metrics(font, "1", 0, false, 0, 0).width
		local with_space = gui.get_text_metrics(font, " 1", 0, false, 0, 0).width
		space_width[font] = with_space - no_space
	end 
	return space_width[font]
end

-- calculate text width with font with respect to trailing space (issue DEF-1761)
local function get_text_width(font, text)
	local result = gui.get_text_metrics(font, text, 0, false, 0, 0).width
	for i=#text, 1, -1 do
		local c = string.sub(text, i, i)
		if c ~= ' ' then
			break
		end
		result = result + get_space_width(font)
	end
	return result
end

local clsUIInput = gwjui.class("UIInput", InputObject)
if false then
	UIInput = clsUIInput
end

function clsUIInput:ctor(params)
	params = params or {}
	clsUIInput.super.ctor(self, params)

	local text = params.text or ""
	local textFont = params.textFont
	local textColor = params.textColor
	self.m_caretColor = params.caretColor or vmath.vector4(0, 0, 0, 1)
	self.m_keyboardType = params.keyboardType or gui.KEYBOARD_TYPE_DEFAULT
	self.m_listener = params.listener
	local mainAnchor = gwjui.getGuiAnchorPoint(self.m_mainNode)
	local mainSize = gwjui.getGuiContentSize(self.m_mainNode)
	gui.set_clipping_mode(self.m_mainNode, gui.CLIPPING_MODE_STENCIL)

	assert(textFont, "textFont must be specified")

	--文字
	self.m_text = gui.new_text_node(vmath.vector3(-mainAnchor.x*mainSize.width, -mainAnchor.y*mainSize.height+mainSize.height/2, 0), text)
	gui.set_pivot(self.m_text, gui.PIVOT_W)
	if(textFont) then
		gui.set_font(self.m_text, textFont)
	end
	if(textColor) then
		gui.set_color(self.m_text, textColor)
	end
	gui.set_parent(self.m_text, self.m_mainNode)
	--光标
	self.m_caret = nil

	self:setTouchEnabled(true)
	self:setTouchEventListener(gwjui.handler(self, self.onTouch_))
	self:setTextEventListener(gwjui.handler(self, self.onText_))
	self:setBackspaceListener(gwjui.handler(self, self.onBackspace_))
end

function clsUIInput:onTouch_(event)
--	gwjui.printf("ontouch:%s", event.name)
	if(event.name == "began") then
		gwjui.callfunc(self.m_listener, "began", self)
	elseif(event.name == "moved") then
	elseif(event.name == "ended") then
		if(gui.pick_node(self.m_mainNode, event.x, event.y)) then
			local old = gwjinput.setCaptureKeyboard(self)
			if(old ~= self) then
				self:enableCaret(true)
				self:setCaretPos()
			end
			gui.reset_keyboard()
			gui.show_keyboard(self.m_keyboardType, true)
		end
	end
end

function clsUIInput:onText_(event)
	local text = gui.get_text(self.m_text)
	text = text .. event.text
	local font = gui.get_font(self.m_text)
	local w = get_text_width(font, text)
	local wh = gwjui.getGuiContentSize(self.m_mainNode)
	if(w <= wh.width) then
		gui.set_text(self.m_text, text)
		self:setCaretPos()
		gwjui.callfunc(self.m_listener, "changed", self)
	end
end

function clsUIInput:onBackspace_(event)
	if(event.pressed or event.repeated) then
--		print("back space")
		local text = gui.get_text(self.m_text)
		local last_s = 0
		for uchar in string.gfind(text, utf8_gfind) do
			last_s = string.len(uchar)
		end
		text = string.sub(text, 1, string.len(text)-last_s)
		gui.set_text(self.m_text, text)
		self:setCaretPos()
		gwjui.callfunc(self.m_listener, "changed", self)
	end
end

function clsUIInput:onReleaseKBCapture()
	self:enableCaret(false)
	gui.hide_keyboard()
	gwjui.callfunc(self.m_listener, "ended", self)
end

function clsUIInput:enableCaret(enable)
	if(enable) then
		if(self.m_caret == nil) then
			local size = gui.get_size(self.m_mainNode)
			local anchor = gwjui.getGuiAnchorPoint(self.m_mainNode)
			self.m_caret = gui.new_box_node(vmath.vector3(-anchor.x*size.x, -anchor.y*size.y+size.y/2, 0), vmath.vector3(3, size.y-8, 0))
			gui.set_color(self.m_caret, self.m_caretColor)
			gui.set_parent(self.m_caret, self.m_mainNode)
		end
	else
		if(self.m_caret) then
			gui.delete_node(self.m_caret)
			self.m_caret = nil
		end
	end
end

function clsUIInput:setCaretPos()
	if(self.m_caret == nil) then
		return
	end
	local font = gui.get_font(self.m_text)
	local text = gui.get_text(self.m_text)
	local width = get_text_width(font, text)
	local size = gui.get_size(self.m_mainNode)
	local anchor = gwjui.getGuiAnchorPoint(self.m_mainNode)
	local x = width + (-anchor.x*size.x)
	gwjui.setGuiPos(self.m_caret, {x=x})

	--光标重新闪烁
	gui.set_color(self.m_caret, self.m_caretColor)
	gui.cancel_animation(self.m_caret, "color.w")
	gui.animate(self.m_caret, "color.w", 0, gui.EASING_INSINE, 0.8, 0, nil, gui.PLAYBACK_LOOP_PINGPONG)
end

function clsUIInput:getText()
	local text = gui.get_text(self.m_text)
	return text
end

return clsUIInput
