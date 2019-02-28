#pragma once

#include <dmsdk/sdk.h>
#include <functional>
#include <extension/luajavaoc/src/LuaValue.h>

extern unsigned int ext_scheduleUpdate(const std::function<void(void)> &func);
extern void ext_unscheduleUpdate(unsigned int entryId);
extern lua_State* ext_getLuaStateWithCallbackId(int callbackId);
extern void ext_pushLuaValueDict(lua_State *L, const LuaValueDict& dict);
extern void ext_pushLuaValueArray(lua_State *L, const LuaValueArray& array);
extern void ext_invokeLuaCallback(int callbackId, int num_args);
extern void ext_unregisterLuaCallback(int callbackId);
