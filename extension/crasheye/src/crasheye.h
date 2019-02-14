#pragma once

#include <dmsdk/sdk.h>
#include <string>

//运营: 1beb2fb0  测试: 334df7e0
#define CRASHEYE_APPKEY_ANDROID_FORMAL	"1beb2fb0"
#define CRASHEYE_APPKEY_ANDROID_TEST	"334df7e0"
//运营: fdf24d90  测试: 711498b0
#define CRASHEYE_APPKEY_IOS_FORMAL		"fdf24d90"
#define CRASHEYE_APPKEY_IOS_TEST		"711498b0"

void crasheye_onAppInit(const char *environment);
int crasheye_sendScriptError(lua_State *L);
int crasheye_setUserId(lua_State *L);
int crasheye_addExtraData(lua_State *L);
int crasheye_removeExtraData(lua_State *L);
int crasheye_clearExtraData(lua_State *L);

extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);