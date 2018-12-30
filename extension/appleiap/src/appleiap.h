#pragma once

#include <dmsdk/sdk.h>

int appleiap_init(lua_State *L);
int appleiap_loadProducts(lua_State *L);
int appleiap_purchase(lua_State *L);
int appleiap_finishTransaction(lua_State *L);

extern void ext_unregisterLuaCallback(int callbackId);
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);