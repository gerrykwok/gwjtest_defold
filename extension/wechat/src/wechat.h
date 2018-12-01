#pragma once

#include <dmsdk/sdk.h>
#include <functional>
#include <string>

void wechat_onAppInit();
void wechat_login();
void wechat_logout();
void wechat_notifyLoginResult(const char *res);
void wechat_onUpdate();
int wechat_shareWithIntent(lua_State *L);
int wechat_shareWithIosSystem(lua_State *L);

//注册一个lua callback
//返回id
extern int ext_registerLuaCallback(lua_State* L, int index);
//根据id反注册callback
extern void ext_unregisterLuaCallback(int callbackId);
//调用callback
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
//调用java代码
extern std::string ext_call_java_static_void_method(const char *clazz, const char *method, const char *signature, ...);
//在update线程中执行代码
extern void ext_performInUpdateThread(const std::function<void(void)> &func);

extern std::string ext_jsonFromLuaTable(lua_State *L, int index);
extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
