// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_IOS)
#import <UIKit/UIKit.h>
#import "TakePhoto.h"

void plm_get_photo(int fromCamera, const char *localPath, int width, int height, int luaCallback)
{
//	NSLog(@"gwjgwj,plm_get_photo,fromCamera=%d,path=%s,width=%d,height=%d,callback=%d", fromCamera, localPath, width, height, luaCallback);
	UIWindow *window = dmGraphics::GetNativeiOSUIWindow();
	UIViewController *controller = window.rootViewController;

	NSString *path = [NSString stringWithUTF8String:localPath];
//	NSLog(@"path:%@", path);

	TakePhoto* takePhoto=[[TakePhoto alloc] init];
	[takePhoto setViewController:controller];
	[takePhoto setSavePath:path];
	takePhoto.m_luaCallback = luaCallback;
	[takePhoto setOutputWidth:width andHeight:height];

	if (fromCamera) {
		[takePhoto snapImageSourceTypeCamera];
	}else{
		[takePhoto snapImageSourceTypePhotoLibrary];
	}
}

#endif
