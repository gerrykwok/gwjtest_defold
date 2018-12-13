#define EXTENSION_NAME xianliao
#define LIB_NAME "xianliao"
#define MODULE_NAME "xianliao"

#include <dmsdk/sdk.h>
#include "xianliao.h"

static const luaL_reg Module_methods[] =
{
	{"login", xianliao_login},
	{"logout", xianliao_logout},
	{"share", xianliao_share},
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
	xianliao_onAppInit();
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

static dmExtension::Result extFinal(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, extAppInit, extAppFinal, extInit, 0, 0, extFinal)
