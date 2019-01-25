#if defined(DM_PLATFORM_IOS)

#include "../misc.h"

int misc_installApk(lua_State *L)
{
	return 0;
}

int misc_canOpenApp(lua_State *L)
{
	return ext_callNativeStaticMethod("MiscFunc", "canOpenApp", L, -1);
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
	lua_pushstring(L, "granted");
	return 1;
}

int misc_androidRequestPermissions(lua_State *L)
{
	return 0;
}

int misc_androidGetTargetSdkVersion(lua_State *L)
{
	lua_pushinteger(L, 0);
	return 1;
}

int misc_androidGotoAppSetting(lua_State *L)
{
	lua_pushstring(L, "not support");
	return 1;
}

#endif
