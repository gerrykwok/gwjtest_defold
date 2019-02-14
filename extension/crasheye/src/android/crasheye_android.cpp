#if defined(DM_PLATFORM_ANDROID)

#include "../crasheye.h"

#define CRASHEYE_CLASS			"com.xishanju.plm.crasheye.CrasheyeUtil"

void crasheye_onAppInit(const char *environment)
{
	bool ok;
	const char *appkey;
	if(strcmp(environment, "development") == 0 || strcmp(environment, "test") == 0)
	{
		appkey = CRASHEYE_APPKEY_ANDROID_TEST;
	}
	else
	{
		appkey = CRASHEYE_APPKEY_ANDROID_FORMAL;
	}
	ext_callJavaStaticMethod(CRASHEYE_CLASS, "init", appkey, &ok);
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
