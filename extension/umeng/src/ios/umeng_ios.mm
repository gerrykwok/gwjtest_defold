#if defined(DM_PLATFORM_IOS)

#include "../umeng.h"
#import <UMCommon/UMCommon.h>
#import <UMAnalytics/MobClick.h>

void umeng_onAppInit()
{
	[MobClick setScenarioType:E_UM_GAME];
	[UMConfigure initWithAppkey:@UMENG_APPKEY_IOS channel:@UMENG_CHANNEL];
}

void umeng_onAppActivate()
{
}

void umeng_onAppDeactivate()
{
}

int umeng_sendEvent(lua_State *L)
{
	return 0;
}

#endif