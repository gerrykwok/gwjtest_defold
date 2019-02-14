#define EXTENSION_NAME crasheye
#define LIB_NAME "crasheye"
#define MODULE_NAME "crasheye"

#include <dmsdk/sdk.h>
#include "crasheye.h"

static bool g_inited = false;

static const luaL_reg Module_methods[] =
{
	{"sendScriptError", crasheye_sendScriptError},
	{"setUserId", crasheye_setUserId},
	{"addExtraData", crasheye_addExtraData},
	{"removeExtraData", crasheye_removeExtraData},
	{"clearExtraData", crasheye_clearExtraData},
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
	if(!g_inited)
	{
		crasheye_onAppInit(dmConfigFile::GetString(params->m_ConfigFile, "project.environment", ""));
		g_inited = true;
	}
	return dmExtension::RESULT_OK;
}

static dmExtension::Result ext_Init(dmExtension::Params* params)
{
	LuaInit(params->m_L);
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

static dmExtension::Result ext_update(dmExtension::Params *params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points:
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, ext_AppInit, ext_AppFinal, ext_Init, ext_update, 0, ext_Final)
