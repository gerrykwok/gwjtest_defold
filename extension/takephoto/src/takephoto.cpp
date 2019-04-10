#define EXTENSION_NAME takephoto
#define LIB_NAME "takephoto"
#define MODULE_NAME "takephoto"

#include <dmsdk/sdk.h>
#include "takephoto.h"

static bool g_appInit = false;

static const luaL_reg Module_methods[] =
{
	{"getPhoto", takephoto_get_photo},
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
	if(!g_appInit)
	{
		g_appInit = true;
		takephoto_onAppInit(dmConfigFile::GetString(params->m_ConfigFile, "project.environment", ""));
	}
	return dmExtension::RESULT_OK;
}

static dmExtension::Result extInit(dmExtension::Params* params)
{
	dmLogInfo("Registered %s Extension", MODULE_NAME);
	LuaInit(params->m_L);
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
