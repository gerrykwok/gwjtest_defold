#if defined(DM_PLATFORM_IOS)

#include "../wechat.h"
#import "PlatformWechat.h"
#import "AppControllerWechat.h"

static AppControllerWechat* g_appDelegate;
void wechat_onAppInit(const char *environment)
{
	if(!g_appDelegate)
	{
		NSString *appid;
		if(strcmp(environment, "development") == 0 || strcmp(environment, "test") == 0) appid = @WECHAT_APPID_TEST;
		else appid = @WECHAT_APPID_FORMAL;

		g_appDelegate = [[AppControllerWechat alloc] init];
		[g_appDelegate setWxAppId:appid];
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
