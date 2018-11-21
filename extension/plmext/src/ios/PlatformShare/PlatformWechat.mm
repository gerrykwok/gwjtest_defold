#if defined(DM_PLATFORM_IOS)

#import "PlatformWechat.h"
#import "../lua/CocosLua.h"
#import "NSString+ShareUtil.h"

#import "../WeChatSDK1.8.3_NoPay/WXApi.h"

@implementation PlatformWechat

+(void)compileIn
{
}

+ (void)login:(NSDictionary *)params {
    int callback = [params[@"callback"] intValue];

    SendAuthReq* req = [[[SendAuthReq alloc]init]autorelease];
    req.scope = @"snsapi_userinfo";
    req.state = @"wechat_login_plm";
    [WXApi sendReq:req];
}

+ (void)logout {
}

@end

#endif//#if defined(DM_PLATFORM_IOS)
