#define LIB_NAME "plmext"
#define MODULE_NAME "plmext"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include "plmext.h"
#include "tolua/tolua++.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "plmext_luastack.h"
#if defined(DM_PLATFORM_ANDROID)
#include "android/CCLuaJavaBridge.h"
#include "android/plmext_android.h"
#endif
#if defined(DM_PLATFORM_IOS)
#include "ios/CCLuaObjcBridge.h"
#include "ios/plmext_ios.h"
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
	plm_set_activity_to_java();
#endif

#if defined(DM_PLATFORM_IOS)
	LuaObjcBridge::luaopen_luaoc(L);
	plm_compile_in();
#endif	
}

dmExtension::Result AppInitializeMyExtension(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result InitializeMyExtension(dmExtension::Params* params)
{
	// Init Lua
	LuaInit(params->m_L);
	printf("Registered %s Extension\n", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

dmExtension::Result AppFinalizeMyExtension(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result FinalizeMyExtension(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(plmext, LIB_NAME, AppInitializeMyExtension, AppFinalizeMyExtension, InitializeMyExtension, 0, 0, FinalizeMyExtension)
