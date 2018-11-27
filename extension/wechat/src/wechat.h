#pragma once

#include <dmsdk/sdk.h>

void wechat_onAppInit();
void wechat_login();
void wechat_logout();
void wechat_notifyLoginResult(const char *res);

//注册一个lua callback
//返回id
extern int ext_registerLuaCallback(lua_State* L, int index);
//根据id反注册callback
extern void ext_unregisterLuaCallback(int callbackId);
//调用callback
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
//调用java代码
extern void ext_call_java_static_void_method(const char *clazz, const char *method, const char *signature, ...);