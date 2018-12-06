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

void wechat_login()
{
	[PlatformWechat login];
}

void wechat_logout()
{
}

int wechat_shareWithIosSystem(lua_State *L)
{
	std::string param = ext_jsonFromLuaTable(L, -1);
	bool ok;
	std::string ret = ext_callOcStaticMethod("ShareUtil", "shareWithSystem", param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

int wechat_shareWithSDK(lua_State *L)
{
	return 0;
}

#endif
