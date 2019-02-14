#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <functional>

//运营: 5c13773bf1f5561c61000343  测试: 5c137bfdf1f556e09600011d
#define UMENG_APPKEY_ANDROID_FORMAL		"5c13773bf1f5561c61000343"
#define UMENG_APPKEY_ANDROID_TEST		"5c137bfdf1f556e09600011d"
//运营: 5c137ca6b465f5560c0000f6  测试: 5c137cdef1f556ba9a00015c
#define UMENG_APPKEY_IOS_FORMAL			"5c137ca6b465f5560c0000f6"
#define UMENG_APPKEY_IOS_TEST			"5c137cdef1f556ba9a00015c"
//channel
#define UMENG_CHANNEL					"plmhd"

void umeng_onAppInit(const char *environment);
void umeng_onAppActivate();
void umeng_onAppDeactivate();
int umeng_sendEvent(lua_State *L);

extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);
extern void ext_performWithDelay(int delayInUpdateCount, const std::function<void(void)> &func);