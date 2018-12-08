#pragma once

#include <dmsdk/sdk.h>
#include <string>

int misc_installApk(lua_State *L);

extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
