#if defined(DM_PLATFORM_IOS)

// include the Defold SDK
#include <dmsdk/sdk.h>
#import <UIKit/UIKit.h>

#import "plmext_ios.h"
#import "TakePhoto.h"
#import "UploadImageBridge.h"
#import "AppController.h"
#import "PlatformShare/PlatformWechat.h"

static AppController* g_appDelegate;
void plmext_onAppInit()
{
	if(!g_appDelegate)
		g_appDelegate = [[AppController alloc] init];
	dmExtension::RegisteriOSUIApplicationDelegate(g_appDelegate);
}

void plm_compile_in()
{
	[TakePhoto compileIn];
	[UploadImageBridge compileIn];
	[PlatformWechat compileIn];
}

#endif
