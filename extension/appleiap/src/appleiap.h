#pragma once

#include <dmsdk/sdk.h>

int appleiap_init(lua_State *L);
int appleiap_loadProducts(lua_State *L);
int appleiap_purchase(lua_State *L);
int appleiap_finishTransaction(lua_State *L);
