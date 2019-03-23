#pragma once

#include <dmsdk/sdk.h>
#include <extension/luajavaoc/src/luacallback.h>

int appleiap_init(lua_State *L);
int appleiap_uninit(lua_State *L);
int appleiap_canMakePurchases(lua_State *L);
int appleiap_loadProducts(lua_State *L);
int appleiap_purchase(lua_State *L);
int appleiap_finishTransaction(lua_State *L);
