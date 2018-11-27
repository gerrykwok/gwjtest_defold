#if defined(DM_PLATFORM_IOS)

#include "../wechat.h"
#import "PlatformWechat.h"
#import "AppControllerWechat.h"

static AppControllerWechat* g_appDelegate;
void wechat_onAppInit()
{
	if(!g_appDelegate)
	{
		g_appDelegate = [[AppControllerWechat alloc] init];
		dmExtension::RegisteriOSUIApplicationDelegate(g_appDelegate);
	}
}

void wechat_login()
{
	[PlatformWechat login];
}

void wechat_logout()
{
}

#endif
