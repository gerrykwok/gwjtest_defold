#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <functional>

//声网appid
#define AGORA_APPID				"96c36b2da95a4bda9b363d04620afd88"

void agora_onAppInit();
int agora_initCallback(lua_State *L);
int agora_joinChannel(lua_State *L);
int agora_leaveChannel(lua_State *L);
int agora_getCurrentChannel(lua_State *L);

extern int ext_registerLuaCallback(lua_State* L, int index);
extern void ext_unregisterLuaCallback(int callbackId);
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
extern void ext_performInUpdateThread(const std::function<void(void)> &func);
extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);
