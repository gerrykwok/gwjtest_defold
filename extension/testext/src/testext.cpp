#define EXTENSION_NAME testext
#define LIB_NAME "testext"
#define MODULE_NAME "testext"

#include <dmsdk/sdk.h>
#include "testext.h"
#include "libbase64.h"

static int test(lua_State *L)
{
	dmLogDebug("gwjgwj,log debug");//不显示
	dmLogError("gwjgwj,log error");//显示成红色
	dmLogFatal("gwjgwj,log fatal");//显示成白色
	dmLogInfo("gwjgwj,log info")//显示成白色
	dmLogUserDebug("gwjgwj,log user debug");//显示成蓝色
	dmLogWarning("gwjgwj,log warning");//显示成橙色

	return 0;
}

static int luatableToJson(lua_State *L)
{
	std::string ret = ext_jsonFromLuaTable(L, -1);
	lua_pushstring(L, ret.c_str());
	return 1;
}

int test_encodeBase64(lua_State *L)
{
	const char *input = lua_tostring(L, -1);
	int inputlength = lua_tointeger(L, -1);
	int bufsize = Base64encode_len(inputlength);
	char *buffer = new char[bufsize];
	int size = Base64encode(buffer, input, inputlength) - 1;
	if(size) lua_pushstring(L, buffer);
	else lua_pushnil(L);
	delete[] buffer;
	return 1;
}

int test_decodeBase64(lua_State *L)
{
	const char *input = lua_tostring(L, -1);
	int bufsize = Base64decode_len(input);
	char *buffer = new char[bufsize];
	int size = Base64decode(buffer, input);
	if(size) lua_pushlstring(L, buffer, size);
	else lua_pushnil(L);
	delete[] buffer;
	return 1;
}

static const luaL_reg Module_methods[] =
{
	{"test", test},
	{"luatableToJson", luatableToJson},
#if defined(DM_PLATFORM_IOS)
	{"takePhoto", test_takephoto},
	{"test_oc", test_oc},
#endif
	{"callnative", test_callnative},
#if defined(DM_PLATFORM_WINDOWS)
	{"printMacro", test_printMacro},
#endif
	{"encodeBase64", test_encodeBase64},
	{"decodeBase64", test_decodeBase64},
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
