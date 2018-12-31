#if defined(DM_PLATFORM_IOS)

#include "../appleiap.h"

int appleiap_init(lua_State *L)
{
	return ext_callNativeStaticMethod("IapUtil", "static_init", L, -1);
}

int appleiap_canMakePurchases(lua_State *L)
{
	ext_callNativeStaticMethod("IapUtil", "static_canMakePurchases", L, -1); /* stack: ok res */
	bool ok = lua_toboolean(L, -2);
	const char *res = lua_tostring(L, -1);
	dmLogInfo("gwjgwj,canMakePurchases,ok=%d,res=%s", ok, res);
	lua_pop(L, 2);
	bool canMakePurchases = false;
	if(!ok) canMakePurchases = false;
	else canMakePurchases = strcmp(res, "yes") == 0;
	lua_pushboolean(L, canMakePurchases);
	return 1;
}

int appleiap_loadProducts(lua_State *L)
{
	return ext_callNativeStaticMethod("IapUtil", "static_loadProducts", L, -1);
}

int appleiap_purchase(lua_State *L)
{
	return ext_callNativeStaticMethod("IapUtil", "static_purchase", L, -1);
}

int appleiap_finishTransaction(lua_State *L)
{
	return ext_callNativeStaticMethod("IapUtil", "static_finishTransaction", L, -1);
}

#endif