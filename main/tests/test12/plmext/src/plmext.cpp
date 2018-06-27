#define LIB_NAME "plmext"
#define MODULE_NAME "plmext"

// include the Defold SDK
#include <dmsdk/sdk.h>
#include "plmext.h"
#include "tolua/tolua++.h"
#include "tolua_fix.h"
#include "LuaBasicConversions.h"
#include "plmext_luastack.h"
#if defined(DM_PLATFORM_ANDROID)
#include "android/LuaJavaBridge.h"
#endif

static int getPhoto(lua_State *L)
{
	int argc;
	tolua_Error tolua_err;

	LuaStack::getInstance()->initWithLuaState(L);

	argc = lua_gettop(L);
	if(argc != 5)
	{
		luaL_error(L, "%s has wrong number of arguments: %d, was expecting %d\n", "getPhoto", argc, 5);
		return 0;
	}

	bool isFromCamera;
	int width, height;
	bool ok;
	ok = luaval_to_boolean(L, 1, &isFromCamera, "getPhoto");
	if(!ok)
	{
		tolua_error(L, "invalid arguments in function 'getPhoto'", NULL);
		return 0;
	}
	int fromCamera = isFromCamera ? 1 : 0;
	const char *localPath = luaL_checkstring(L, 2);
	width = luaL_checkint(L, 3);
	height = luaL_checkint(L, 4);

	int idxFunc = 5;
	if(!toluafix_isfunction(L,idxFunc,"LUA_FUNCTION",0,&tolua_err))
	{
		tolua_error(L,"#ferror in function 'getPhoto'.",&tolua_err);
		return 0;
	}
	LUA_FUNCTION luaCallback = toluafix_ref_function(L,idxFunc,0);

	//printf("getPhoto,localPath=%s,luaCallback=%d\n", localPath, luaCallback);
	plm_get_photo(fromCamera, localPath, width, height, luaCallback);
	return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
	{"getPhoto", getPhoto},
	{0, 0}
};

static void LuaInit(lua_State* L)
{
	int top = lua_gettop(L);
#if defined(DM_PLATFORM_ANDROID)
	LuaJavaBridge_ensureCompileIn();//为了安卓上编译时能把jni函数编译进so
#endif

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
