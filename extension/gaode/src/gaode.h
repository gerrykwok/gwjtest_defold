#pragma once

#include <dmsdk/sdk.h>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <extension/luajavaoc/src/luacallback.h>
#include <extension/httpreq/src/httpreq.h>

#define GAODE_APIKEY_ANDROID_FORMAL				"e4afaac26e14fbe2b05cff032632ced4"
#define GAODE_ANDROID_PACKAGE_FORMAL			"com.xsjplm.plm"
#define GAODE_APIKEY_ANDROID_TEST				"4a585d5f9e0bb877239e8e0f322283f4"
#define GAODE_ANDROID_PACKAGE_TEST				"com.xsjplm.plmtest"
#define GAODE_APIKEY_IOS_FORMAL					"1dbecf26da251a537565b7a5f4f3dd8c"
#define GAODE_APIKEY_WEB						"8714a8e66fe2a8ed9e3167ebf8219395"

void gaode_onAppInit(const char *environment);
int gaode_locationInit(lua_State *L);
int gaode_locationStart(lua_State *L);
int gaode_locationStop(lua_State *L);
int gaode_getAuthorizationStatus(lua_State *L);
int gaode_jumpLocationSetting(lua_State *L);

void gaode_requestLocationByIp(int callback);
