#if defined(DM_PLATFORM_IOS)

#include "../appleiap.h"

int appleiap_init(lua_State *L)
{
	return ext_callNativeStaticMethod("IapUtil", "static_init", L, -1);
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