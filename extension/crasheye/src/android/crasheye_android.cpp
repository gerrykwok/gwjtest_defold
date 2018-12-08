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
	std::string param = ext_jsonFromLuaTable(L, -1);
	bool ok;
	std::string ret = ext_callJavaStaticMethod(CRASHEYE_CLASS, "sendScriptError", param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

int crasheye_setUserId(lua_State *L)
{
	const char *userId = lua_tostring(L, -1);
	bool ok;
	std::string ret = ext_callJavaStaticMethod(CRASHEYE_CLASS, "setUserId", userId, &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

int crasheye_addExtraData(lua_State *L)
{
	std::string param = ext_jsonFromLuaTable(L, -1);
	bool ok;
	std::string ret = ext_callJavaStaticMethod(CRASHEYE_CLASS, "addExtraData", param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

#endif
