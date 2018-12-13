#if defined(DM_PLATFORM_IOS)

#import "AppControllerXianliao.h"
#import "../xianliao.h"

@implementation AppControllerXianliao

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	return YES;
}

//-(BOOL) application:(UIApplication *)application handleOpenURL:(NSURL *)url
//{
//	return [WXApi handleOpenURL:url delegate:self];
//}
//
//-(BOOL) application:(UIApplication *)application openURL:(NSURL *)url sourceApplication:(NSString *)sourceApplication annotation:(id)annotation
//{
//	return [WXApi handleOpenURL:url delegate:self];
//}

//用此方法，上面两个方法就不会被回调
-(BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options
{
	return [SugramApiManager handleOpenURL:url];
}

@end

#endif
