#pragma once

#include <dmsdk/sdk.h>

int appleiap_init(lua_State *L);
int appleiap_uninit(lua_State *L);
int appleiap_canMakePurchases(lua_State *L);
int appleiap_loadProducts(lua_State *L);
int appleiap_purchase(lua_State *L);
int appleiap_finishTransaction(lua_State *L);

extern void ext_unregisterLuaCallback(int callbackId);
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);

extern char* crypto_encode_base64(const void *inData, int dataLen);
extern void* crypto_decode_base64(const char *codedString, int *decodeSize);
extern void crypto_delete_buffer(void *buffer);
