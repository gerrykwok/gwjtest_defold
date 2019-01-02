#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <functional>

int test_oc(lua_State *L);
int test_takephoto(lua_State *L);
int test_callnative(lua_State *L);
int test_printMacro(lua_State *L);

//根据id反注册callback
extern void ext_unregisterLuaCallback(int callbackId);
//调用callback
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
//在update线程中执行代码
extern void ext_performInUpdateThread(const std::function<void(void)> &func);

extern std::string ext_jsonFromLuaTable(lua_State *L, int index);
extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);

extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);
