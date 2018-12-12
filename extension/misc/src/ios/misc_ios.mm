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

#endif
