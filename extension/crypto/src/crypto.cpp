#define EXTENSION_NAME crypto
#define LIB_NAME "crypto"
#define MODULE_NAME "crypto"

#include <dmsdk/sdk.h>
#include "crypto.h"

int crypto_encodeBase64(lua_State *L)
{
	const char *input = lua_tostring(L, -1);
	int inputlength = lua_tointeger(L, -1);

	char* encoded = crypto_encode_base64(input, inputlength);

	if(encoded == NULL) lua_pushnil(L);
	else lua_pushstring(L, encoded);
	crypto_delete_buffer(encoded);
	return 1;
}

int crypto_decodeBase64(lua_State *L)
{
	const char *input = lua_tostring(L, -1);

	int decodeSize;
	void* decoded = crypto_decode_base64(input, &decodeSize);

	if(decoded == NULL) lua_pushnil(L);
	else lua_pushlstring(L, (char*)decoded, decodeSize);
	crypto_delete_buffer(decoded);
	return 1;
}

static const luaL_reg Module_methods[] =
{
	{"encodeBase64", crypto_encodeBase64},
	{"decodeBase64", crypto_decodeBase64},
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
