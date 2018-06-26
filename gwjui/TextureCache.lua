local gwjui = require("gwjui.gwjui_impl")

local clsTextureCache = gwjui.class("TextureCache")
clsTextureCache.s_allInst = {}

function clsTextureCache.getInstance()
	local url = tostring(msg.url())
	if(clsTextureCache.s_allInst[url] == nil) then
		clsTextureCache.s_allInst[url] = clsTextureCache.new()
	end
	return clsTextureCache.s_allInst[url]
end

function clsTextureCache:ctor()
	self.m_allTexture = {}
end

function clsTextureCache:final()
	local ok,res = pcall(function()
		for k,id in pairs(self.m_allTexture) do
			gui.delete_texture(k)
		end
	end)
	if(not ok) then
		print(res)
	end
	self.m_allTexture = {}
	local url = tostring(msg.url())
	clsTextureCache.s_allInst[url] = nil
end

--返回texture id
--如果加载不成功则返回nil
function clsTextureCache:addImage(filepath)
	if(type(filepath) ~= "string") then
		return nil
	end
	if(self.m_allTexture[filepath]) then--has previously loaded
		return filepath
	end
	local file = io.open(filepath, "rb")
	if(file) then
		local data = file:read("*a")
		file:close()

		local img = image.load(data)
		local texture_id = filepath
		if gui.new_texture(texture_id, img.width, img.height, img.type, img.buffer) then
			self.m_allTexture[texture_id] = true
			return texture_id
		else
			gwjui.printf("gwjgwj,gui.new_texture error")
		end
	end
end

function clsTextureCache:removeTextureForKey(key)
	if(not self.m_allTexture[key]) then
		return
	end
	self.m_allTexture[key] = nil
	gwjui.printf("gwjgwj,delete texture:%s", key)
	gui.delete_texture(key)
end

return clsTextureCache
