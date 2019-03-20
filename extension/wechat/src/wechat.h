#pragma once

#include <dmsdk/sdk.h>
#include <functional>
#include <string>
#include <extension/luajavaoc/src/luacallback.h>

//微信后台信息, 用于微信登录和微信SDK分享
//测试   appid: wx4452e2c2c7b10700  appsecret: 82b127e817ba3e091f101c02e04e8b3d
//运营   appid: wxefac673b77470b4f  appsecret: e9d61b2042f52d576de07a0829dbf595
#define WECHAT_APPID_FORMAL		"wxefac673b77470b4f"
#define WECHAT_APPID_TEST		"wx4452e2c2c7b10700"

void wechat_onAppInit(const char *environment);
int wechat_login(lua_State *L);
int wechat_logout(lua_State *L);
//void wechat_notifyLoginResult(const char *res);
void wechat_onUpdate();
int wechat_shareWithIntent(lua_State *L);
int wechat_shareWithIosSystem(lua_State *L);
int wechat_shareWithSDK(lua_State *L);
int wechat_makePurchase(lua_State *L);
