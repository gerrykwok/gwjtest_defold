#define LIB_NAME "plmext"
#define MODULE_NAME "plmext"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include "plmext.h"
#include "tolua/tolua++.h"
#include "tolua_fix.h"

static int getLocalPicture(lua_State *L)
{
	int argc;
	tolua_Error tolua_err;

	argc = lua_gettop(L);
	if(argc != 2)
	{
		luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d\n", "getLocalPicture", argc, 2);
		return 0;
	}

	const char *localPath = luaL_checkstring(L, 1);
	if(!toluafix_isfunction(L,2,"LUA_FUNCTION",0,&tolua_err))
	{
		tolua_error(L,"#ferror in function 'getLocalPicture'.",&tolua_err);
		return 0;
	}
	LUA_FUNCTION luaCallback = toluafix_ref_function(L,2,0);

	printf("getLocalPicture,localPath=%s,luaCallback=%d\n", localPath, luaCallback);
	plm_get_local_picture(localPath, luaCallback);
	return 0;
}

static int takePhoto(lua_State *L)
{
	int argc;
	tolua_Error tolua_err;

	argc = lua_gettop(L);
	if(argc != 2)
	{
		luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d\n", "getLocalPicture", argc, 2);
		return 0;
	}

	const char *localPath = luaL_checkstring(L, 1);
	if(!toluafix_isfunction(L,2,"LUA_FUNCTION",0,&tolua_err))
	{
		tolua_error(L,"#ferror in function 'getLocalPicture'.",&tolua_err);
		return 0;
	}
	LUA_FUNCTION luaCallback = toluafix_ref_function(L,2,0);

	plm_take_photo(localPath, luaCallback);
	return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
	{"getLocalPicture", getLocalPicture},
	{"takePhoto", takePhoto},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);

	// Register lua names
	luaL_register(L, MODULE_NAME, Module_methods);
	toluafix_open(L);

	lua_pop(L, 1);
	assert(top == lua_gettop(L));
}

dmExtension::Result AppInitializeMyExtension(dmExtension::AppParams* params)
{
	printf("gwjgwj,app_init\n");
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
//
// DM_DECLARE_EXTENSION(symbol, name, app_init, app_final, init, update, on_event, final)

DM_DECLARE_EXTENSION(plmext, LIB_NAME, AppInitializeMyExtension, AppFinalizeMyExtension, InitializeMyExtension, 0, 0, FinalizeMyExtension)
