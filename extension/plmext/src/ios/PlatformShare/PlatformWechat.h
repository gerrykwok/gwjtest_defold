
#import <Foundation/Foundation.h>

@interface PlatformWechat : NSObject
{
}

/**
 * 微信登录
 */
+ (void)login:(NSDictionary *)params;

/**
 * 微信登出
 */
+ (void)logout;

/**
 * 判断客户端是否安装微信
 */
+ (int)isClientInstalled;

/**
 * 微信分享
 */
+ (void)share:(NSDictionary *)params;

@end
