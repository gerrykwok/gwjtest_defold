#if defined(DM_PLATFORM_IOS)

#include "../umeng.h"
#import "UmengUtil.h"

void umeng_onAppInit(const char *environment)
{
	NSString *appkey;
	NSString *channel;
	channel = @UMENG_CHANNEL;
	if(strcmp(environment, "development") == 0 || strcmp(environment, "test") == 0) appkey = @UMENG_APPKEY_IOS_TEST;
	else appkey = @UMENG_APPKEY_IOS_FORMAL;
	[UmengUtil init:@{
		@"appkey": appkey,
		@"channel": channel,
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