// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_WINDOWS)

static int test(lua_State *L)
{
	return 0;
}

// Functions exposed to Lua
static const luaL_reg Module_methods[] =
{
	{"test", test},
	{0, 0}
};

void plmext_win32_init(lua_State *L)
{
//	int top = lua_gettop(L);
	// Register lua names
	luaL_register(L, "plmextWin32", Module_methods);
	lua_pop(L, 1);
}

#endif
