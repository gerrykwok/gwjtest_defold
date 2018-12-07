#define EXTENSION_NAME wechat
#define LIB_NAME "wechat"
#define MODULE_NAME "wechat"

//微信extension，包含微信登录、微信分享、微信支付

#include "wechat.h"

static int g_wechatLoginCallbackId = -1;

static int wechatLogin(lua_State *L)
{
	g_wechatLoginCallbackId = ext_registerLuaCallback(L, 1);
	dmLogInfo("callbackid = %d", g_wechatLoginCallbackId);
	wechat_login();
	return 0;
}

static int wechatLogout(lua_State *L)
{
	wechat_logout();
	return 0;
}

void wechat_notifyLoginResult(const char *res)
{
	if(g_wechatLoginCallbackId >= 0)
	{
		ext_invokeLuaCallbackWithString(g_wechatLoginCallbackId, res);
		ext_unregisterLuaCallback(g_wechatLoginCallbackId);
		g_wechatLoginCallbackId = -1;
	}
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
	{"login", wechatLogin},
	{"logout", wechatLogout},
#if defined(DM_PLATFORM_ANDROID)
	{"shareWithIntent", wechat_shareWithIntent},
#elif defined(DM_PLATFORM_IOS)
	{"shareWithIosSystem", wechat_shareWithIosSystem},
#endif
	{"shareWithSDK", wechat_shareWithSDK},
	{"makePurchase", wechat_makePurchase},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	// Register lua names
	luaL_register(L, MODULE_NAME, Module_methods);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

static dmExtension::Result ext_AppInit(dmExtension::AppParams* params)
{
	wechat_onAppInit();
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Init(dmExtension::Params* params)
{
	LuaInit(params->m_L);
	dmLogInfo("Registered %s Extension", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_AppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Final(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, ext_AppInit, ext_AppFinal, ext_Init, 0, 0, ext_Final)
