#if defined(DM_PLATFORM_ANDROID)

#include "../crasheye.h"

#define CRASHEYE_CLASS			"com.xishanju.plm.crasheye.CrasheyeUtil"

void crasheye_onAppInit()
{
	bool ok;
	ext_callJavaStaticMethod(CRASHEYE_CLASS, "init", CRASHEYE_APPKEY_ANDROID, &ok);
}

int crasheye_sendScriptError(lua_State *L)
{
	return ext_callNativeStaticMethod(CRASHEYE_CLASS, "sendScriptError", L, -1);
}

int crasheye_setUserId(lua_State *L)
{
	return ext_callNativeStaticMethod(CRASHEYE_CLASS, "setUserId", L, -1);
}

int crasheye_addExtraData(lua_State *L)
{
	return ext_callNativeStaticMethod(CRASHEYE_CLASS, "addExtraData", L, -1);
}

int crasheye_removeExtraData(lua_State *L)
{
	return ext_callNativeStaticMethod(CRASHEYE_CLASS, "removeExtraData", L, -1);
}

int crasheye_clearExtraData(lua_State *L)
{
	return ext_callNativeStaticMethod(CRASHEYE_CLASS, "clearExtraData", L, -1);
}

#endif
