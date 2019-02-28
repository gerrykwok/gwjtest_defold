#pragma once

#include <dmsdk/sdk.h>
#include <functional>
#include <string>

#include "LuaValue.h"

//注册一个lua callback
//返回id
int ext_registerLuaCallback(lua_State* L, int index);
//根据id反注册callback
void ext_unregisterLuaCallback(int callbackId);
//调用callback
void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
//获取某个callback对应的lua状态机
lua_State* ext_getLuaStateWithCallbackId(int callbackId);
//用指定参数个数调用callback(需先push相应的值)
void ext_invokeLuaCallback(int callbackId, int num_args);

void ext_pushLuaValue(lua_State *L, const LuaValue& value);
void ext_pushLuaValueDict(lua_State *L, const LuaValueDict& dict);
void ext_pushLuaValueArray(lua_State *L, const LuaValueArray& array);

//在update线程中执行代码(执行一次)
void ext_performInUpdateThread(const std::function<void(void)> &func);
//把代码加入到update线程中执行(一直执行)
//返回ID(<=0代表无效ID)
unsigned int ext_scheduleUpdate(const std::function<void(void)> &func);
//移除在update中执行的代码
void ext_unscheduleUpdate(unsigned int entryId);
//延迟执行代码
void ext_performWithDelay(int delayInUpdateCount, const std::function<void(void)> &func);
//根据lua栈中的参数转换为json字符串
//idxParam指向参数的lua栈索引，是个table
std::string ext_jsonFromLuaTable(lua_State *L, int index);

//调用java方法
//java方法型如: String func(Context, String)
//如果返回的ok为true，则该函数返回java方法的返回值，否则，返回出错消息
std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
//调用oc方法
std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);

std::string ext_callNativeStaticMethodBase(const char *clazz, const char *method, lua_State *L, int idxParam, bool *ok);

//调用原生方法(java或oc)
//传送到原生方法的参数是一个lua table，位于idxParam的lua栈中
//java方法型如: String func(Context, JSONObject)
//OC方法型如: +(NSString*) func:(NSDictionary*)params  返回值可任意
//返回: 插入到lua栈的返回值个数
int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);
