#ifndef _COCOSEXT_ANDROID_H_
#define _COCOSEXT_ANDROID_H_

void cocosext_android_init();

int LuaJavaBridge_callLuaFunctionById(int functionId, const char *value);
int LuaJavaBridge_callLuaGlobalFunction(const char *luaFunctionName, const char *value);
int LuaJavaBridge_retainLuaFunctionById(int luaFunctionId);
int LuaJavaBridge_releaseLuaFunctionById(int luaFunctionId);

#endif