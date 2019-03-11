#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <functional>
#include <extension/luajavaoc/src/luacallback.h>

//声网appid
#define AGORA_APPID				"96c36b2da95a4bda9b363d04620afd88"

void agora_onAppInit();
int agora_initCallback(lua_State *L);
int agora_joinChannel(lua_State *L);
int agora_leaveChannel(lua_State *L);
int agora_getCurrentChannel(lua_State *L);
