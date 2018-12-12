#pragma once

#include <dmsdk/sdk.h>
#include <string>

int misc_installApk(lua_State *L);
int misc_isApkInstalled(lua_State *L);
int misc_canOpenApp(lua_State *L);

extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);
