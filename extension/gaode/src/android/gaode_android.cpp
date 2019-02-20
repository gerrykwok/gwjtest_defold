#if defined(DM_PLATFORM_ANDROID)

#include <stdlib.h>
#include "../gaode.h"

extern void ext_callLuaCallbackInAndroid(JNIEnv *env, jint callback, jstring value, bool unregister);

void gaode_onAppInit(const char *environment)
{
}

int gaode_locationInit(lua_State *L)
{
	bool ok;
	const char *apiKey;
	std::string packageName = ext_callJavaStaticMethod("com.xishanju.plm.gaode.GaodeUtil", "getPackageName", "", &ok);
	dmLogInfo("package=%s", packageName.c_str());
	if(packageName == GAODE_ANDROID_PACKAGE_FORMAL)
		apiKey = GAODE_APIKEY_ANDROID_FORMAL;
	else if(packageName == GAODE_ANDROID_PACKAGE_TEST)
		apiKey = GAODE_APIKEY_ANDROID_TEST;
	else apiKey = GAODE_APIKEY_ANDROID_FORMAL;
	std::string res = ext_callJavaStaticMethod("com.xishanju.plm.gaode.GaodeUtil", "locationInit", apiKey, &ok);

	lua_pushstring(L, res.c_str());
	return 1;
}

int gaode_locationStart(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.gaode.GaodeUtil", "locationStart", L, -1);
}

int gaode_locationStop(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.gaode.GaodeUtil", "locationStop", L, -1);
}

//com.xishanju.plm.gaode
extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_gaode_GaodeUtil_nativeNotifyLocation(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, false);
}

//com.xishanju.plm.gaode
extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_gaode_GaodeUtil_nativeReleaseLuaCallback(JNIEnv *env, jclass clz, jint callback)
{
	ext_unregisterLuaCallback(callback);
}

int gaode_getAuthorizationStatus(lua_State *L)
{
	bool ok;
	std::string res = ext_callJavaStaticMethod("com.xishanju.plm.gaode.GaodeUtil", "getAuthorizationStatus", "", &ok);
	lua_pushinteger(L, atoi(res.c_str()));
	return 1;
}

int gaode_jumpLocationSetting(lua_State *L)
{
	bool ok;
	std::string res = ext_callJavaStaticMethod("com.xishanju.plm.gaode.GaodeUtil", "jumpLocationSetting", "", &ok);
	return 0;
}

#endif