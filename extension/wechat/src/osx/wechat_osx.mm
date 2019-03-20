#if defined(DM_PLATFORM_OSX)

#include "../wechat.h"

void wechat_onAppInit(const char *environment)
{
}

int wechat_login(lua_State *L)
{
	return 0;
}

int wechat_logout(lua_State *L)
{
	return 0;
}

int wechat_shareWithSDK(lua_State *L)
{
	return 0;
}

int wechat_makePurchase(lua_State *L)
{
	return 0;
}

#endif
