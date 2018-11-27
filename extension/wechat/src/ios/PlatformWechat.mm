#if defined(DM_PLATFORM_IOS)

#import "PlatformWechat.h"
#import "WeChatSDK1.8.3_NoPay/WXApi.h"
#import "../wechat.h"

@implementation PlatformWechat

+ (void)login {
    SendAuthReq* req = [[[SendAuthReq alloc]init]autorelease];
    req.scope = @"snsapi_userinfo";
    req.state = @"wechat_login_plm";
    [WXApi sendReq:req];
}

+ (void)logout {
}

+ (void)notifyLoginResult:(int)errCode errStr:(NSString*)errStr code:(NSString*)code
{
    NSString *res = [NSString stringWithFormat:@"{\"errCode\":%d, \"errStr\":\"%@\", \"code\":\"%@\"}", errCode, errStr, code];
    wechat_notifyLoginResult([res UTF8String]);
}

@end

#endif//#if defined(DM_PLATFORM_IOS)
