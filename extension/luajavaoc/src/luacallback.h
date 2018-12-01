#pragma once

#include <functional>
#include <string>

//注册一个lua callback
//返回id
int ext_registerLuaCallback(lua_State* L, int index);
//根据id反注册callback
void ext_unregisterLuaCallback(int callbackId);
//调用callback
void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
//调用java代码
//返回: success或者出错信息
std::string ext_call_java_static_void_method(const char *clazz, const char *method, const char *signature, ...);
//在update线程中执行代码
void ext_performInUpdateThread(const std::function<void(void)> &func);
//根据lua栈中的参数转换为json字符串
//idxParam指向参数的lua栈索引，是个table
std::string ext_jsonFromLuaTable(lua_State *L, int index);

//调用java方法
//java方法型如: String func(Context, String)
//如果返回的ok为true，则该函数返回java方法的返回值，否则，返回出错消息
std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
//调用oc方法
std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
