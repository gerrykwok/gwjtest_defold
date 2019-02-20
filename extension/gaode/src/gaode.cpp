#define EXTENSION_NAME gaode
#define LIB_NAME "gaode"
#define MODULE_NAME "gaode"

#include <dmsdk/sdk.h>
#include "gaode.h"

static bool g_inited = false;

static const luaL_reg Module_methods[] =
{
	{"locationInit", gaode_locationInit},
	{"locationStart", gaode_locationStart},
	{"locationStop", gaode_locationStop},
	{"getAuthorizationStatus", gaode_getAuthorizationStatus},
	{"jumpLocationSetting", gaode_jumpLocationSetting},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);
	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

static dmExtension::Result extAppInit(dmExtension::AppParams* params)
{
	if(!g_inited)
	{
		gaode_onAppInit(dmConfigFile::GetString(params->m_ConfigFile, "project.environment", ""));
		g_inited = true;
	}
	return dmExtension::RESULT_OK;
}

static dmExtension::Result extInit(dmExtension::Params* params)
{
	LuaInit(params->m_L);
	dmLogInfo("Registered %s Extension", MODULE_NAME);
	return dmExtension::RESULT_OK;
}

static dmExtension::Result extAppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static void extOnEvent(dmExtension::Params* params, const dmExtension::Event* event)
{
}

static dmExtension::Result extFinal(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, extAppInit, extAppFinal, extInit, 0, extOnEvent, extFinal)
