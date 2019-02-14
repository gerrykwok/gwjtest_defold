#if defined(DM_PLATFORM_IOS)

#include "../xianliao.h"
#import "XianliaoUtil.h"
#import "AppControllerXianliao.h"

void xianliao_onAppInit(const char *environment)
{
	NSString *appid;
	if(strcmp(environment, "development") == 0 || strcmp(environment, "test") == 0) appid = @XIANLIAO_APPID_TEST;
	else appid = @XIANLIAO_APPID_FORMAL;
	[XianliaoUtil initXianliao:appid];
	AppControllerXianliao *delegate = [[AppControllerXianliao alloc] init];
	dmExtension::RegisteriOSUIApplicationDelegate(delegate);
}

int xianliao_login(lua_State *L)
{
	return ext_callNativeStaticMethod("XianliaoUtil", "login", L, -1);
}

int xianliao_logout(lua_State *L)
{
	return 0;
}

int xianliao_share(lua_State *L)
{
	return ext_callNativeStaticMethod("XianliaoUtil", "share", L, -1);
}

#endif