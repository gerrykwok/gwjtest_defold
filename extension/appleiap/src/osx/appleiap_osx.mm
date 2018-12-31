#if defined(DM_PLATFORM_OSX)

#include "../appleiap.h"

int appleiap_init(lua_State *L)
{
	return 0;
}

int appleiap_canMakePurchases(lua_State *L)
{
	return 0;
}

int appleiap_loadProducts(lua_State *L)
{
	return 0;
}

int appleiap_purchase(lua_State *L)
{
	return 0;
}

int appleiap_finishTransaction(lua_State *L)
{
	return 0;
}

#endif