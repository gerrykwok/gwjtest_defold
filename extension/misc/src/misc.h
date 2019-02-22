#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <vector>

int misc_installApk(lua_State *L);
int misc_isApkInstalled(lua_State *L);
int misc_canOpenApp(lua_State *L);
int misc_androidCheckPermission(lua_State *L);
int misc_androidRequestPermissions(lua_State *L);
int misc_androidGetTargetSdkVersion(lua_State *L);
int misc_androidGotoAppSetting(lua_State *L);
int misc_getBatteryInfo(lua_State *L);
int misc_setAppEventListener(lua_State *L);
int misc_getCurrentThreadId(lua_State *L);
void misc_invokeAppEventListener(const char *res);

extern int ext_registerLuaCallback(lua_State* L, int index);
extern void ext_unregisterLuaCallback(int callbackId);
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);
