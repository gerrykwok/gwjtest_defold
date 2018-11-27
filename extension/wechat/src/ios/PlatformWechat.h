
#import <Foundation/Foundation.h>

@interface PlatformWechat : NSObject
{
}

/**
 * 微信登录
 */
+ (void)login;

/**
 * 微信登出
 */
+ (void)logout;

+ (void)notifyLoginResult:(int)errCode errStr:(NSString*)errStr code:(NSString*)code;

@end
