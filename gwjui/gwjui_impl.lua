local gwjui = {}

function gwjui.class(classname, super)
	local superType = type(super)
	local cls

	if superType ~= "function" and superType ~= "table" then
		superType = nil
		super = nil
	end

	if superType == "function" or (super and super.__ctype == 1) then
		-- inherited from native C++ Object
		cls = {}

		if superType == "table" then
			-- copy fields from super
			for k,v in pairs(super) do cls[k] = v end
			cls.__create = super.__create
			cls.super    = super
		else
			cls.__create = super
			cls.ctor = function() end
		end

		cls.__cname = classname
		cls.__ctype = 1

		function cls.new(...)
			local instance = cls.__create(...)
			-- copy fields from class to native object
			for k,v in pairs(cls) do instance[k] = v end
			instance.class = cls
			instance:ctor(...)
			return instance
		end

	else
		-- inherited from Lua Object
		if super then
			cls = {}
			setmetatable(cls, {__index = super})
			cls.super = super
		else
			cls = {ctor = function() end}
		end

		cls.__cname = classname
		cls.__ctype = 2 -- lua
		cls.__index = cls

		function cls.new(...)
			local instance = setmetatable({}, cls)
			instance.class = cls
			instance:ctor(...)
			return instance
		end
	end

	function cls.createInstance(...)
		return cls.new(...)
	end

	return cls
end

--[[--

如果对象是指定类或其子类的实例，返回 true，否则返回 false

~~~ lua

local Animal = class("Animal")
local Duck = class("Duck", Animal)

print(iskindof(Duck.new(), "Animal")) -- 输出 true

~~~

@param mixed obj 要检查的对象
@param string classname 类名

@return boolean

]]
function gwjui.iskindof(obj, classname)
	local t = type(obj)
	local mt
	if t == "table" then
		mt = getmetatable(obj)
	elseif t == "userdata" then
		mt = tolua.getpeer(obj)
	end

	while mt do
		if mt.__cname == classname then
			return true
		end
		mt = mt.super
	end

	return false
end

function gwjui.clone(object)
	local lookup_table = {}
	local function _copy(object)
		if type(object) ~= "table" then
			return object
		elseif lookup_table[object] then
			return lookup_table[object]
		end
		local new_table = {}
		lookup_table[object] = new_table
		for key, value in pairs(object) do
			new_table[_copy(key)] = _copy(value)
		end
		return setmetatable(new_table, getmetatable(object))
	end
	return _copy(object)
end

function gwjui.point(x, y)
	return {x=x,y=y}
end

function gwjui.size(w, h)
	return {width=w, height=h}
end

function gwjui.rect(x, y, w, h)
	return {x=x, y=y, width=w, height=h}
end

function gwjui.rectContainsPoint(rect, point)
	local ret = false
	if (point.x >= rect.x) and (point.x <= rect.x + rect.width) and
		(point.y >= rect.y) and (point.y <= rect.y + rect.height) then
		ret = true
	end
	return ret
end

--模仿cocos2dx的getAnchorPoint
function gwjui.getGuiAnchorPoint(node)
	local pivot = gui.get_pivot(node)
	if(pivot == gui.PIVOT_CENTER) then
		return gwjui.point(0.5, 0.5)
	elseif(pivot == gui.PIVOT_N) then
		return gwjui.point(0.5, 1)
	elseif(pivot == gui.PIVOT_NE) then
		return gwjui.point(1, 1)
	elseif(pivot == gui.PIVOT_E) then
		return gwjui.point(1, 0.5)
	elseif(pivot == gui.PIVOT_SE) then
		return gwjui.point(1, 0)
	elseif(pivot == gui.PIVOT_S) then
		return gwjui.point(0.5, 0)
	elseif(pivot == gui.PIVOT_SW) then
		return gwjui.point(0, 0)
	elseif(pivot == gui.PIVOT_W) then
		return gwjui.point(0, 0.5)
	elseif(pivot == gui.PIVOT_NW) then
		return gwjui.point(0, 1)
	end
end

function gwjui.getGuiContentSize(node)
	local size = gui.get_size(node)
	return gwjui.size(size.x, size.y)
end

--仅设定GUI节点的X或Y
function gwjui.setGuiPos(node, pos)
	pos = pos or {}
	local p = gui.get_position(node)
	if(pos.x) then p.x = pos.x end
	if(pos.y) then p.y = pos.y end
	gui.set_position(node, p)
end

function gwjui.printf(...)
	print(string.format(...))
end

local function string_split(input, delimiter)
	input = tostring(input)
	delimiter = tostring(delimiter)
	if (delimiter=='') then return false end
	local pos,arr = 0, {}
	-- for each divider found
	for st,sp in function() return string.find(input, delimiter, pos, true) end do
		table.insert(arr, string.sub(input, pos, st - 1))
		pos = sp + 1
	end
	table.insert(arr, string.sub(input, pos))
	return arr
end

local function string_trim(input)
	input = string.gsub(input, "^[ \t\n\r]+", "")
	return string.gsub(input, "[ \t\n\r]+$", "")
end

--[[--

输出值的内容

### 用法示例

~~~ lua

local t = {comp = "chukong", engine = "quick"}

dump(t)

~~~

@param mixed value 要输出的值

@param [string desciption] 输出内容前的文字描述

@parma [integer nesting] 输出时的嵌套层级，默认为 3

]]
function gwjui.dump(value, desciption, nesting)
	if type(nesting) ~= "number" then nesting = 3 end

	local lookupTable = {}
	local result = {}

	local function _v(v)
		if type(v) == "string" then
			v = "\"" .. v .. "\""
		end
		return tostring(v)
	end

	local traceback = string_split(debug.traceback("", 2), "\n")
	print("dump from: " .. string_trim(traceback[3]))

	local function _dump(value, desciption, indent, nest, keylen)
		desciption = desciption or "<var>"
		spc = ""
		if type(keylen) == "number" then
			spc = string.rep(" ", keylen - string.len(_v(desciption)))
		end
		if type(value) ~= "table" then
			result[#result +1 ] = string.format("%s%s%s = %s", indent, _v(desciption), spc, _v(value))
		elseif lookupTable[value] then
			result[#result +1 ] = string.format("%s%s%s = *REF*", indent, desciption, spc)
		else
			lookupTable[value] = true
			if nest > nesting then
				result[#result +1 ] = string.format("%s%s = *MAX NESTING*", indent, desciption)
			else
				result[#result +1 ] = string.format("%s%s = {", indent, _v(desciption))
				local indent2 = indent.."    "
				local keys = {}
				local keylen = 0
				local values = {}
				for k, v in pairs(value) do
					keys[#keys + 1] = k
					local vk = _v(k)
					local vkl = string.len(vk)
					if vkl > keylen then keylen = vkl end
					values[k] = v
				end
				table.sort(keys, function(a, b)
					if type(a) == "number" and type(b) == "number" then
						return a < b
					else
						return tostring(a) < tostring(b)
					end
				end)
				for i, k in ipairs(keys) do
					_dump(values[k], k, indent2, nest + 1, keylen)
				end
				result[#result +1] = string.format("%s}", indent)
			end
		end
	end
	_dump(value, desciption, "- ", 1)

	for i, line in ipairs(result) do
		print(line)
	end
end

-- Convert string to hash, unless it's already a hash
-- @param str String to convert
-- @return The hashed string
function gwjui.to_hash(str)
	return type(str) == "string" and hash(str) or str
end

-- safely call function
function gwjui.callfunc(func, ...)
	if(func) then
		func(...)
	end
end

function gwjui.handler(obj, method)
	return function(...)
		return method(obj, ...)
	end
end

return gwjui
