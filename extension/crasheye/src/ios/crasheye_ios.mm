#if defined(DM_PLATFORM_IOS)

#include "../crasheye.h"
#include "crasheye_ios_v2.6.3/Crasheye.h"

void crasheye_onAppInit()
{
	NSLog(@"gwjgwj,init crasheye with key %s", CRASHEYE_APPKEY_IOS);
	[Crasheye initWithAppKey:@CRASHEYE_APPKEY_IOS];
}

int crasheye_sendScriptError(lua_State *L)
{
	std::string param = ext_jsonFromLuaTable(L, -1);
	bool ok;
	std::string ret = ext_callOcStaticMethod("CrasheyeUtil", "sendScriptError", param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

int crasheye_setUserId(lua_State *L)
{
	const char *userId = lua_tostring(L, -1);
	std::string param = (std::string)"{\"userId\":\"" + userId + "\"}";
	bool ok;
	std::string ret = ext_callOcStaticMethod("CrasheyeUtil", "setUserId", param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

int crasheye_addExtraData(lua_State *L)
{
	std::string param = ext_jsonFromLuaTable(L, -1);
	bool ok;
	std::string ret = ext_callOcStaticMethod("CrasheyeUtil", "addExtraData", param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

#endif
