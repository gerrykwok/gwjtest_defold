--author: guoweiji
--date : 2019-04-22
--version : defold 1.2.151
--desc: 由所有的extension得到的lua api

misc = {}

--- <summary>
--- params: a table which has path
--- </summary>
function misc.installApk(params) end

--- <summary>
--- params: a table which has package
--- </summary>
function misc.isApkInstalled(params) end

--- <summary>
--- params: a table which has url
--- </summary>
function misc.canOpenApp(params) end

--- <summary>
--- get path in bundle
--- </summary>
function misc.getBundleResourcePath(filename) end

--- <summary>
--- check permission
--- return 'granted' or 'denied'
--- </summary>
function misc.androidCheckPermission(permission) end

--- <summary>
--- request some permissions
--- params: a table, has:
---       permissions: an array of permissions
---       callback: function to tell the request result
--- </summary>
function misc.androidRequestPermissions(params) end

--- <summary>
--- get android:targetSdkVersion in Androidmanifest.xml
--- return a number
--- </summary>
function misc.androidGetTargetSdkVersion() end

--- <summary>
--- goto the setting of an app
--- </summary>
function misc.androidGotoAppSetting(package) end

--- <summary>
--- return level and charing
--- </summary>
function misc.getBatteryInfo() end

--- <summary>
--- set an app event listener, I.E. app switch to background and foreground
--- </summary>
function misc.setAppEventListener(listener) end

--- <summary>
--- return current thread id
--- </summary>
function misc.getCurrentThreadId() end

--- <summary>
--- vibrate for a period of time
--- </summary>
function misc.vibrate(seconds) end

--- <summary>
--- go to desktop home
--- </summary>
function misc.goHome() end

--- <summary>
--- do print
--- </summary>
function misc.print(msg) end
