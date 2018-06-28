#define LIB_NAME "plmext"
#define MODULE_NAME "plmext"

// include the Defold SDK
#include <dmsdk/sdk.h>
#if defined(DM_PLATFORM_ANDROID)
#endif
#if defined(DM_PLATFORM_IOS)
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

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
#if defined(DM_PLATFORM_ANDROID)
#endif

#if defined(DM_PLATFORM_IOS)
	plm_compile_in();
#endif
}

dmExtension::Result plmext_AppInit(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result plmext_Init(dmExtension::Params* params)
{
	// Init Lua
	LuaInit(params->m_L);
	printf("Registered %s Extension\n", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

dmExtension::Result plmext_AppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

dmExtension::Result plmext_Final(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(plmext, LIB_NAME, plmext_AppInit, plmext_AppFinal, plmext_Init, 0, 0, plmext_Final)
