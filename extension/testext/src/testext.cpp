#define EXTENSION_NAME testext
#define LIB_NAME "testext"
#define MODULE_NAME "testext"

#include <dmsdk/sdk.h>
//#define DLIB_LOG_DOMAIN "MyOwnDomain"
#include <dmsdk/dlib/log.h>

static int test(lua_State *L)
{
	dmLogDebug("gwjgwj,test,lua_state=0x%x", L);
	printf("fffff,lua_state=0x%lx\n", (unsigned long)L);
	return 0;
}

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
}

static dmExtension::Result ext_AppInit(dmExtension::AppParams* params)
{
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
