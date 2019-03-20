#if defined(DM_PLATFORM_IOS)

#import "AppControllerWechat.h"
#import "PlatformWechat.h"
#import "ShareUtil.h"
#import "../wechat.h"

@implementation AppControllerWechat

-(void)setWxAppId:(NSString*)appid
{
	m_appid = appid;
}

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
	[WXApi registerApp:m_appid];
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

//用此方法，上面两个方法就不会被回调
-(BOOL)application:(UIApplication *)app openURL:(NSURL *)url options:(NSDictionary<UIApplicationOpenURLOptionsKey,id> *)options
{
	return [WXApi handleOpenURL:url delegate:self];
}

-(void) onReq:(BaseReq*)reqonReq
{
}

-(void) onResp:(BaseResp*)resp
{
	if([resp isKindOfClass:[SendAuthResp class]])
	{
		SendAuthResp *authResp = (SendAuthResp*)resp;
		// NSLog(@"gwjgwj,onResp:code:%@,state:%@,lang:%@,country:%@", authResp.code, authResp.state, authResp.lang, authResp.country);
		NSString *res = [NSString stringWithFormat:@"{\"errCode\":%d, \"errStr\":\"%@\", \"code\":\"%@\"}", resp.errCode, resp.errStr, authResp.code];
		[PlatformWechat notifyLoginResult:res];
	}
	else if([resp isKindOfClass:[SendMessageToWXResp class]])
	{
		NSString *result = @"";
		switch(resp.errCode)
		{
		case WXSuccess:
			result = @"success";
			break;
		case WXErrCodeUserCancel:
			result = @"cancel";
			break;
		default:
			result = @"error";
			break;
		}
		NSString *res = [NSString stringWithFormat:@"{\"result\":\"%@\"}", result];
		[ShareUtil notifyShareResult:res];
	}
}

@end

#endif
