#if defined(DM_PLATFORM_ANDROID)

#include "../misc.h"

int misc_installApk(lua_State *L)
{
	const char *apkPath = lua_tostring(L, -1);
	bool ok;
	std::string ret = ext_callJavaStaticMethod("com.xishanju.plm.misc.MiscFunc", "startInstallApk", apkPath, &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

#endif
