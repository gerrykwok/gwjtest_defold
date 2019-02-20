#if defined(DM_PLATFORM_WINDOWS)

#include "../gaode.h"

void gaode_onAppInit(const char *environment)
{
}

int gaode_locationInit(lua_State *L)
{
	return 0;
}

int gaode_locationStart(lua_State *L)
{
	return 0;
}

int gaode_locationStop(lua_State *L)
{
	return 0;
}

int gaode_getAuthorizationStatus(lua_State *L)
{
	lua_pushinteger(L, 2);
	return 1;
}

int gaode_jumpLocationSetting(lua_State *L)
{
	return 0;
}

#endif