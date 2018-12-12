#if defined(DM_PLATFORM_IOS)

#include "../wechat.h"
#import "PlatformWechat.h"
#import "AppControllerWechat.h"

static AppControllerWechat* g_appDelegate;
void wechat_onAppInit()
{
	if(!g_appDelegate)
	{
		g_appDelegate = [[AppControllerWechat alloc] init];
		dmExtension::RegisteriOSUIApplicationDelegate(g_appDelegate);
	}
}

int wechat_login(lua_State *L)
{
	return ext_callNativeStaticMethod("PlatformWechat", "login", L, -1);
}

int wechat_logout(lua_State *L)
{
	return ext_callNativeStaticMethod("PlatformWechat", "logout", L, -1);
}

int wechat_shareWithIosSystem(lua_State *L)
{
	return ext_callNativeStaticMethod("ShareUtil", "shareWithSystem", L, -1);
}

int wechat_shareWithSDK(lua_State *L)
{
	return ext_callNativeStaticMethod("ShareUtil", "shareWithSDK", L, -1);
}

int wechat_makePurchase(lua_State *L)
{
	return 0;
}

#endif
