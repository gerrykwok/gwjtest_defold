#if defined(DM_PLATFORM_IOS)

#include "../gaode.h"
#import "GaodeUtil.h"

void gaode_onAppInit(const char *environment)
{
}

int gaode_locationInit(lua_State *L)
{
	return ext_callNativeStaticMethod("GaodeUtil", "locationInit", L, -1);
}

int gaode_locationStart(lua_State *L)
{
	return ext_callNativeStaticMethod("GaodeUtil", "locationStart", L, -1);
}

int gaode_locationStop(lua_State *L)
{
	return ext_callNativeStaticMethod("GaodeUtil", "locationStop", L, -1);
}

int gaode_getAuthorizationStatus(lua_State *L)
{
	return ext_callNativeStaticMethod("GaodeUtil", "getLocationAuthorizationStatus", L, -1);
}

int gaode_jumpLocationSetting(lua_State *L)
{
	return ext_callNativeStaticMethod("GaodeUtil", "jumpLocationSetting", L, -1);
}

#endif