#pragma once

//注册一个lua callback
//返回id
int ext_registerLuaCallback(lua_State* L, int index);
//根据id反注册callback
void ext_unregisterLuaCallback(int callbackId);
//调用callback
void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
//调用java代码
void ext_call_java_static_void_method(const char *clazz, const char *method, const char *signature, ...);
