#pragma once

#include <dmsdk/sdk.h>
#include <string>

//运营: 1beb2fb0  测试: 334df7e0
#define CRASHEYE_APPKEY_ANDROID			"334df7e0"
//运营: fdf24d90  测试: 711498b0
#define CRASHEYE_APPKEY_IOS				"711498b0"

void crasheye_onAppInit();
int crasheye_sendScriptError(lua_State *L);
int crasheye_setUserId(lua_State *L);
int crasheye_addExtraData(lua_State *L);

extern std::string ext_jsonFromLuaTable(lua_State *L, int index);
extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
