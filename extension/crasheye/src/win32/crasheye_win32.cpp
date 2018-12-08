#if defined(DM_PLATFORM_WINDOWS)

#include "../crasheye.h"

void crasheye_onAppInit()
{
}

int crasheye_sendScriptError(lua_State *L)
{
	return 0;
}

int crasheye_setUserId(lua_State *L)
{
	return 0;
}

int crasheye_addExtraData(lua_State *L)
{
	return 0;
}

#endif
