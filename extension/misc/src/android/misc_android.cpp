#if defined(DM_PLATFORM_ANDROID)

#include "../misc.h"

int misc_installApk(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.misc.MiscFunc", "startInstallApk", L, -1);
}

#endif
