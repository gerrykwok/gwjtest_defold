#define LIB_NAME "cocosext"
#define MODULE_NAME "cocosext"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include "cocosext.h"
#include "tolua/tolua++.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "CCLuaStack.h"
#if defined(DM_PLATFORM_ANDROID)
#include "android/CCLuaJavaBridge.h"
#include "android/cocosext_android.h"
#endif
#if defined(DM_PLATFORM_IOS)
#include "ios/CCLuaObjcBridge.h"
#include "plmext/ios/plmext_ios.h"
#endif
#if defined(DM_PLATFORM_WINDOWS)
#include "plmext/win32/plmext_win32.h"
#endif

static int test(lua_State *L)
{
	return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
	{"test", test},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	// Register lua names
	luaL_register(L, MODULE_NAME, Module_methods);
	toluafix_open(L);
	LuaStack::getInstance()->initWithLuaState(L);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
#if defined(DM_PLATFORM_ANDROID)
	LuaJavaBridge::luaopen_luaj(L);
	cocosext_set_activity_to_java();
#endif

#if defined(DM_PLATFORM_IOS)
	LuaObjcBridge::luaopen_luaoc(L);
	plm_compile_in();
#endif

#if defined(DM_PLATFORM_WINDOWS)
	plmext_win32_init(L);
#endif
}

dmExtension::Result cocosext_AppInit(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result cocosext_Init(dmExtension::Params* params)
{
	// Init Lua
	LuaInit(params->m_L);
	printf("Registered %s Extension\n", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

dmExtension::Result cocosext_AppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result cocosext_Final(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(cocosext, LIB_NAME, cocosext_AppInit, cocosext_AppFinal, cocosext_Init, 0, 0, cocosext_Final)
