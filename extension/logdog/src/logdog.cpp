#define EXTENSION_NAME logdog
#define LIB_NAME "logdog"
#define MODULE_NAME "logdog"

#include <dmsdk/sdk.h>
#include <string>
#include "logdog.h"

static bool g_appInited = false;

int lua_print(lua_State * luastate)
{
	int nargs = lua_gettop(luastate);
	
	std::string t;
	for(int i = 1; i <= nargs; i++)
	{
		if(lua_istable(luastate, i))
			t += "table";
		else if(lua_isnone(luastate, i))
			t += "none";
		else if(lua_isnil(luastate, i))
			t += "nil";
		else if(lua_isboolean(luastate, i))
		{
			if(lua_toboolean(luastate, i) != 0)
				t += "true";
			else
				t += "false";
		}
		else if(lua_isfunction(luastate, i))
			t += "function";
		else if(lua_islightuserdata(luastate, i))
			t += "lightuserdata";
		else if(lua_isthread(luastate, i))
			t += "thread";
		else
		{
			const char * str = lua_tostring(luastate, i);
			if(str)
				t += lua_tostring(luastate, i);
			else
				t += lua_typename(luastate, lua_type(luastate, i));
		}
		if(i != nargs)
			t += "\t";
	}
//	t = (std::string)"[LUA-print] " + t;
	logdog_doLog(t.c_str());

	return 0;
}

static const luaL_reg Module_methods[] =
{
	{"log", logdog_lualog},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);

	luaL_register(L, MODULE_NAME, Module_methods);
	lua_pop(L, 1);

	//重载lua的print
	const luaL_reg global_functions[] = {
		{ "print", lua_print },
		{ nullptr, nullptr }
	};
	luaL_register(L, "_G", global_functions);
	lua_pop(L, 1);

	assert(top == lua_gettop(L));
}

int logdog_lualog(lua_State *L)
{
	const char *msg = lua_tostring(L, -1);
	logdog_doLog(msg);
	return 0;
}

static dmExtension::Result extAppInit(dmExtension::AppParams* params)
{
	if(!g_appInited)
	{
		logdog_onAppInit();
		g_appInited = true;
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
