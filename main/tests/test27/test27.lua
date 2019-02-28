local gwjui = require("gwjui.gwjui")
local TipsBanner = require("main.common.tipsbanner.tipsbanner")

local test27 = gwjui.class("test27")

function test27:ctor()
end

function test27:onEnter()
	gwjui.ScaleButton.new({
		main_id = "btn_unzip_onefile",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickUnzipOneFile()
	end)
	gwjui.ScaleButton.new({
		main_id = "btn_unzip_allfile",
		maxScale = 1.1,
	})
	:onButtonClicked(function()
		self:onClickUnzipAllFile()
	end)
end

function test27:onExit()
end

function test27:on_message(message_id, message, sender)
end

function test27:getZipFile()
	local zipfile
	gwjui.printf("device.platform=%s", tostring(device.platform))
	if(device.platform == "android") then
		zipfile = "/mnt/sdcard/photos.zip"
	elseif(device.platform == "ios") then
		zipfile = "photos.zip"
	elseif(device.platform == "windows") then
		zipfile = "D:\\defold\\projects\\gwjtest\\photos.zip"
	elseif(device.platform == "mac") then
		zipfile = "photos.zip"
	end
	return zipfile
end

function test27:onClickUnzipOneFile()
	local zipfile = self:getZipFile()
	local zip = unzip.UnzipUtil:create(zipfile)

	local pathInZip = "wallpaper/one_piece/9b7fdc32c1c942df3103859497738fbc.jpg"
	local targetPath = sys.get_save_file(sys.get_config("project.title"), "unzip_out.jpg")
	local needPassword = false
	local password = ""

	gwjui.printf("gwjgwj,start extract one file,targetPath=%s", targetPath)
--	zip:unzipOneFile(pathInZip, targetPath, needPassword, password, nil)--同步
	zip:unzipOneFile(pathInZip, targetPath, needPassword, password, function(script, event)--异步
		gwjui.dump(event, "event")
		if(event.name == "completed") then
			gwjui.printf("gwjgwj,completed")
			TipsBanner.show("unzip completed")
		end
	end)
	gwjui.printf("gwjgwj,start extract one file end")
end

function test27:onClickUnzipAllFile()
	local zipfile = self:getZipFile()
	local zip = unzip.UnzipUtil:create(zipfile)

	local pathInZip = "wallpaper/one_piece/9b7fdc32c1c942df3103859497738fbc.jpg"
	local targetDir = sys.get_save_file(sys.get_config("project.title"), "unzipout")
	local needPassword = false
	local password = ""

	lfs.mkdir(targetDir)
	gwjui.printf("gwjgwj,start extract %s", zipfile)
--	zip:unzipAllFile(targetDir, false, "", nil)--同步
	zip:unzipAllFile(targetDir, needPassword, password, function(script, event)--异步
--		gwjui.dump(event, "event")
		if(event.name == "progress") then
			gwjui.printf("gwjgwj,extracting %d/%d", event.current, event.filenum)
		elseif(event.name == "completed") then
			gwjui.printf("gwjgwj,extract completed")
		end
	end)
	gwjui.printf("gwjgwj,start extract end")
end

return test27
