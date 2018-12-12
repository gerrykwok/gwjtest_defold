#if defined(DM_PLATFORM_IOS)

#import "PlatformWechat.h"
#import "WeChatSDK1.8.3_NoPay/WXApi.h"
#import "../wechat.h"

static int g_loginCallback = 0;

@implementation PlatformWechat

+ (void)login:(NSDictionary*)params
{
    g_loginCallback = [params[@"callback"] intValue];

    SendAuthReq* req = [[[SendAuthReq alloc]init]autorelease];
    req.scope = @"snsapi_userinfo";
    req.state = @"wechat_login_plm";
    [WXApi sendReq:req];
}

+ (void)logout:(NSDictionary*)params
{
}

+ (void)notifyLoginResult:(NSString*)res
{
    if(g_loginCallback > 0)
    {
        ext_invokeLuaCallbackWithString(g_loginCallback, [res UTF8String]);
        ext_unregisterLuaCallback(g_loginCallback);
        g_loginCallback = 0;
    }
}

@end

#endif//#if defined(DM_PLATFORM_IOS)
