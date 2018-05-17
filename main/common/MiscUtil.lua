function ccprint(...)
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
function dump(value, desciption, nesting)
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

cc={}

function cc.size(width, height)
	return {width=width, height=height}
end

function cc.p(x, y)
	return {x=x, y=y}
end

function cc.rect(x, y, w, h)
	return {x=x, y=y, w=w, h=h}
end

local winSize = cc.size(tonumber(sys.get_config("display.width")) or 960, tonumber(sys.get_config("display.height")) or 640)
display = {}
display.size               = {width = winSize.width, height = winSize.height}
display.width              = display.size.width
display.height             = display.size.height
display.cx                 = display.width / 2
display.cy                 = display.height / 2
display.c_left             = -display.width / 2
display.c_right            = display.width / 2
display.c_top              = display.height / 2
display.c_bottom           = -display.height / 2
display.left               = 0
display.right              = display.width
display.top                = display.height
display.bottom             = 0
--display.widthInPixels      = display.sizeInPixels.width
--display.heightInPixels     = display.sizeInPixels.height
ccprint("display:%s,%s", tostring(display.width), tostring(display.height))
function setDisplayWHInPixels(w, h)
	display.widthInPixels = w
	display.heightInPixels = h
end

device = {}
device.platform = "unknown"
local info = sys.get_sys_info()
local sysName = info.system_name
--ccprint("gwjgwj,system_name=%s", sysName)
if(sysName == "Android") then
	device.platform = "android"
elseif(sysName == "iPhone OS") then
	device.platform = "ios"
elseif(sysName == "Windows") then
	device.platform = "windows"
elseif(sysName == "Darwin") then
	device.platform = "mac"
end

local MiscUtil = {}
MiscUtil.table = {}

-- start --

--------------------------------
-- 从表格中查找指定值，返回其索引，如果没找到返回 false
-- @function [parent=#table] indexof
-- @param table array 表格
-- @param mixed value 要查找的值
-- @param integer begin 起始索引值
-- @return integer#integer 

--[[--

从表格中查找指定值，返回其索引，如果没找到返回 false

~~~ lua

local array = {"a", "b", "c"}
print(table.indexof(array, "b")) -- 输出 2

~~~

]]

-- end --

function MiscUtil.table.indexof(array, value, begin)
	for i = begin or 1, #array do
		if array[i] == value then return i end
	end
	return false
end

-- start --

--------------------------------
-- 从表格中查找指定值，返回其 key，如果没找到返回 nil
-- @function [parent=#table] keyof
-- @param table hashtable 表格
-- @param mixed value 要查找的值
-- @return string#string  该值对应的 key

--[[--

从表格中查找指定值，返回其 key，如果没找到返回 nil

~~~ lua

local hashtable = {name = "dualface", comp = "chukong"}
print(table.keyof(hashtable, "chukong")) -- 输出 comp

~~~

]]

-- end --

function MiscUtil.table.keyof(hashtable, value)
	for k, v in pairs(hashtable) do
		if v == value then return k end
	end
	return nil
end

-- start --

--------------------------------
-- 从表格中删除指定值，返回删除的值的个数
-- @function [parent=#table] removebyvalue
-- @param table array 表格
-- @param mixed value 要删除的值
-- @param boolean removeall 是否删除所有相同的值
-- @return integer#integer 

--[[--

从表格中删除指定值，返回删除的值的个数

~~~ lua

local array = {"a", "b", "c", "c"}
print(table.removebyvalue(array, "c", true)) -- 输出 2

~~~

]]

-- end --

function MiscUtil.table.removebyvalue(array, value, removeall)
	local c, i, max = 0, 1, #array
	while i <= max do
		if array[i] == value then
			table.remove(array, i)
			c = c + 1
			i = i - 1
			max = max - 1
			if not removeall then break end
		end
		i = i + 1
	end
	return c
end

return MiscUtil
