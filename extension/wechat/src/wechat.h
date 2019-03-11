#pragma once

#include <dmsdk/sdk.h>
#include <functional>
#include <string>
#include <extension/luajavaoc/src/luacallback.h>

//微信appid，用于微信登录和微信SDK分享
//牌乐门: wxc0acf2068b5dd7a3  牌乐门HD: wxefac673b77470b4f
#define WECHAT_APPID			"wxefac673b77470b4f"

void wechat_onAppInit();
int wechat_login(lua_State *L);
int wechat_logout(lua_State *L);
//void wechat_notifyLoginResult(const char *res);
void wechat_onUpdate();
int wechat_shareWithIntent(lua_State *L);
int wechat_shareWithIosSystem(lua_State *L);
int wechat_shareWithSDK(lua_State *L);
int wechat_makePurchase(lua_State *L);
