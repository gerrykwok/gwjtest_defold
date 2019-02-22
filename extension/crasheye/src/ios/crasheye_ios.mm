#if defined(DM_PLATFORM_IOS)

#include "../crasheye.h"
#include "crasheye_ios_v2.6.3/Crasheye.h"
#import "CrasheyeUtil.h"

extern NSDictionary* ext_NSDictionaryFromLuaTable(lua_State *L, int index);

void crasheye_onAppInit(const char *environment)
{
	NSString *appkey;
	if(strcmp(environment, "development") == 0 || strcmp(environment, "test") == 0)
		appkey = @CRASHEYE_APPKEY_IOS_TEST;
	else appkey = @CRASHEYE_APPKEY_IOS_FORMAL;
//	NSLog(@"gwjgwj,init crasheye with key %@", appkey);
	[Crasheye initWithAppKey:appkey];
}

int crasheye_sendScriptError(lua_State *L)
{
	return ext_callNativeStaticMethod("CrasheyeUtil", "sendScriptError", L, -1);
}

int crasheye_setUserId(lua_State *L)
{
	return ext_callNativeStaticMethod("CrasheyeUtil", "setUserId", L, -1);
}

int crasheye_addExtraData(lua_State *L)
{
	return ext_callNativeStaticMethod("CrasheyeUtil", "addExtraData", L, -1);
}

int crasheye_removeExtraData(lua_State *L)
{
	return ext_callNativeStaticMethod("CrasheyeUtil", "removeExtraData", L, -1);
}

int crasheye_clearExtraData(lua_State *L)
{
	return ext_callNativeStaticMethod("CrasheyeUtil", "clearExtraData", L, -1);
}

#endif
