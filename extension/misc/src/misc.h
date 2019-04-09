#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <vector>
#include <extension/luajavaoc/src/luacallback.h>

int misc_installApk(lua_State *L);
int misc_isApkInstalled(lua_State *L);
int misc_canOpenApp(lua_State *L);
int misc_getBundleResourcePath(lua_State *L);
int misc_androidCheckPermission(lua_State *L);
int misc_androidRequestPermissions(lua_State *L);
int misc_androidGetTargetSdkVersion(lua_State *L);
int misc_androidGotoAppSetting(lua_State *L);
int misc_getBatteryInfo(lua_State *L);
int misc_setAppEventListener(lua_State *L);
int misc_getCurrentThreadId(lua_State *L);
int misc_vibrate(lua_State *L);
int misc_goHome(lua_State *L);

void misc_doVibrate(float timeInSeconds);
void misc_invokeAppEventListener(const char *res);
