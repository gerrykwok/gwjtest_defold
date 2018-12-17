#if defined(DM_PLATFORM_IOS)

#include "../umeng.h"
#import "UmengUtil.h"

void umeng_onAppInit()
{
	[UmengUtil init:@{
		@"appkey": @UMENG_APPKEY_IOS,
		@"channel": @UMENG_CHANNEL,
	}];
}

void umeng_onAppActivate()
{
}

void umeng_onAppDeactivate()
{
}

int umeng_sendEvent(lua_State *L)
{
	return ext_callNativeStaticMethod("UmengUtil", "sendEvent", L, -1);
}

#endif