#if defined(DM_PLATFORM_IOS)

#import "AppController.h"

@implementation AppController

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	//	NSLog(@"gwjgwj,didFinishLaunchingWithOptions");
	NSString *wechatAppId = @"wxc0acf2068b5dd7a3";
	[WXApi registerApp:wechatAppId];
	return YES;
}

-(BOOL) application:(UIApplication *)application handleOpenURL:(NSURL *)url
{
	return [WXApi handleOpenURL:url delegate:self];
}

-(BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
{
	return [WXApi handleOpenURL:url delegate:self];
}

-(void) onReq:(BaseReq*)reqonReq
{
}

-(void) onResp:(BaseResp*)resp
{
	SendAuthResp *authResp = (SendAuthResp*)resp;
	NSLog(@"gwjgwj,onResp:%@", authResp);
}

@end

#endif
