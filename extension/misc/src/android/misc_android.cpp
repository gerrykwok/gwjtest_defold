#if defined(DM_PLATFORM_ANDROID)

#include "../misc.h"
#include <stdlib.h>

int misc_installApk(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.misc.MiscFunc", "startInstallApk", L, -1);
}

int misc_isApkInstalled(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.misc.MiscFunc", "isAppInstalled", L, -1);
}

int misc_androidCheckPermission(lua_State *L)
{
	if(!lua_isstring(L, -1))
	{
		dmLogError("invalid param 1 to androidCheckPermission, expecting string");
		lua_pushstring(L, "denied");
		return 1;
	}
	const char *permission = lua_tostring(L, -1);
	bool ok;
	int ret = 0;
	std::string res = ext_callJavaStaticMethod("com.xishanju.plm.misc.MiscFunc", "checkPermission", permission, &ok);
	lua_pushstring(L, res.c_str());
	return 1;
}

int misc_androidRequestPermissions(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.misc.MiscFunc", "requestPermissions", L, -1);
}

extern void ext_callLuaCallbackInAndroid(JNIEnv *env, jint callback, jstring value, bool unregister);

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_misc_MiscFunc_nativeNotifyRequestPermissionResult(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, true);
}

int misc_androidGetTargetSdkVersion(lua_State *L)
{
	bool ok;
	std::string res = ext_callJavaStaticMethod("com.xishanju.plm.misc.MiscFunc", "getTargetSdkVersion", "", &ok);
	int ver = atoi(res.c_str());
	lua_pushinteger(L, ver);
	return 1;
}

int misc_androidGotoAppSetting(lua_State *L)
{
	const char *package = "";
	if(lua_gettop(L) > 0 && lua_isstring(L, -1))
		package = lua_tostring(L, -1);
	bool ok;
	std::string res = ext_callJavaStaticMethod("com.xishanju.plm.misc.MiscFunc", "gotoAppSetting", package, &ok);
	lua_pushstring(L, res.c_str());
	return 1;
}

int misc_getBatteryInfo(lua_State *L)
{
	bool ok;
	std::string res = ext_callJavaStaticMethod("com.xishanju.plm.misc.MiscFunc", "getBatteryInfo", "", &ok);
	int level, charging;
	sscanf(res.c_str(), "level:%d,charging:%d", &level, &charging);

	lua_pushinteger(L, level);
	lua_pushinteger(L, charging);
	return 2;
}

#endif
