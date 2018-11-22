#if defined(DM_PLATFORM_IOS)

#import "PlatformWechat.h"
#import "../lua/CocosLua.h"
#import "NSString+ShareUtil.h"
#import "../CCLuaBridge.h"

#import "../WeChatSDK1.8.3_NoPay/WXApi.h"

static int g_wechatLoginCallback = -1;

@implementation PlatformWechat

+(void)compileIn
{
}

+ (void)login:(NSDictionary *)params {
    g_wechatLoginCallback = [params[@"callback"] intValue];

    SendAuthReq* req = [[[SendAuthReq alloc]init]autorelease];
    req.scope = @"snsapi_userinfo";
    req.state = @"wechat_login_plm";
    [WXApi sendReq:req];
}

+ (void)logout {
}

+ (void)notifyLoginResult:(int)errCode errStr:(NSString*)errStr code:(NSString*)code
{
    if (g_wechatLoginCallback >= 0)
    {
        NSString *res = [NSString stringWithFormat:@"{\"errCode\":%d, \"errStr\":\"%@\", \"code\":\"%@\"}", errCode, errStr, code];
        LuaBridge::pushLuaFunctionById(g_wechatLoginCallback);
        LuaBridge::getStack()->pushString([res UTF8String]);
        LuaBridge::getStack()->executeFunction(1);
        LuaBridge::getStack()->removeScriptHandler(g_wechatLoginCallback);
        g_wechatLoginCallback = -1;
    }
}

@end

#endif//#if defined(DM_PLATFORM_IOS)
