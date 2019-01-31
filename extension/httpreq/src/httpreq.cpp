#define EXTENSION_NAME httpreq
#define LIB_NAME "httpreq"
#define MODULE_NAME "httpreq"

#include <dmsdk/sdk.h>
#include "httpreq.h"
#include "luabindings/lua_httpreq_auto.hpp"

static int setCurlProxy(lua_State *L)
{
	if(!lua_isstring(L, -1))
	{
		luaL_error(L, "%s: must specify a string", "setProxy");
		return 0;
	}
	const char *proxy = lua_tostring(L, -1);
	httpreq_setCurlProxy(proxy);
	return 0;
}

static const luaL_reg Module_methods[] =
{
	{"setProxy", setCurlProxy},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
	luaL_register(L, MODULE_NAME, Module_methods);
	lua_pop(L, 1);

	register_all_httpreq(L);
	
	assert(top == lua_gettop(L));
}

static dmExtension::Result extAppInit(dmExtension::AppParams* params)
{
	httpreq_printCurlVersion();
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
}

static dmExtension::Result extFinal(dmExtension::Params* params)
{
	return dmExtension::RESULT_OK;
}

// Defold SDK uses a macro for setting up extension entry points
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)
DM_DECLARE_EXTENSION(EXTENSION_NAME, LIB_NAME, extAppInit, extAppFinal, extInit, 0, extOnEvent, extFinal)
