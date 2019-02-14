#define EXTENSION_NAME umeng
#define LIB_NAME "umeng"
#define MODULE_NAME "umeng"

#include <dmsdk/sdk.h>
#include "umeng.h"

static bool g_inited = false;

static const luaL_reg Module_methods[] =
{
	{"sendEvent", umeng_sendEvent},
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
		umeng_onAppInit(dmConfigFile::GetString(params->m_ConfigFile, "project.environment", ""));
		g_inited = true;
	}
	return dmExtension::RESULT_OK;
}

static dmExtension::Result extInit(dmExtension::Params* params)
{
	LuaInit(params->m_L);
	return dmExtension::RESULT_OK;
}

static dmExtension::Result extAppFinal(dmExtension::AppParams* params)
{
	return dmExtension::RESULT_OK;
}

static void extOnEvent(dmExtension::Params* params, const dmExtension::Event* event)
{
	dmExtension::EventID eventId = event->m_Event;
	switch(eventId)
	{
	case dmExtension::EVENT_ID_ACTIVATEAPP:
		dmLogInfo("gwjgwj,activate app");
		umeng_onAppActivate();
		break;
	case dmExtension::EVENT_ID_DEACTIVATEAPP:
		dmLogInfo("gwjgwj,deactivate app");
		umeng_onAppDeactivate();
		break;
	}
}

static dmExtension::Result extFinal(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, extAppInit, extAppFinal, extInit, 0, extOnEvent, extFinal)
