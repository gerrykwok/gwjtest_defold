// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_IOS)
#import <UIKit/UIKit.h>

#import "plmext_ios.h"
#import "TakePhoto.h"
#import "UploadImageBridge.h"

void plm_compile_in()
{
	[TakePhoto compileIn];
	[UploadImageBridge compileIn];
}

#endif