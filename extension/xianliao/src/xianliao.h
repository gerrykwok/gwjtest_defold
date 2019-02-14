#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <functional>

//闲聊appid，用于闲聊登录和闲聊分享
//运营:
//appId：Rcy9A8UaRqA9Xg2w
//appSecret：3ERtKSsTodip96Tl
//ios PackageName：com.xsjplm.plm
//android PackageName：com.xsjplm.plm
//测试:
//appId：Bablaj5yD0cu7sPP
//appSecret：RmwUlmCmVNnU1jjl
//ios PackageName：com.xsjplm.plmtest
//android PackageName：com.xsjplm.plmtest
#define XIANLIAO_APPID_FORMAL		"Rcy9A8UaRqA9Xg2w"
#define XIANLIAO_APPID_TEST			"Bablaj5yD0cu7sPP"

void xianliao_onAppInit(const char *environment);
int xianliao_login(lua_State *L);
int xianliao_logout(lua_State *L);
int xianliao_share(lua_State *L);

extern void ext_unregisterLuaCallback(int callbackId);
extern void ext_invokeLuaCallbackWithString(int callbackId, const char *value);
extern void ext_performInUpdateThread(const std::function<void(void)> &func);
extern std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok);
extern int ext_callNativeStaticMethod(const char *clazz, const char *method, lua_State *L, int idxParam);
