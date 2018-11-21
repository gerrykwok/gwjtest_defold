
#import <Foundation/Foundation.h>

@interface PlatformWechat : NSObject
{
}

/**
* compile in
*/
+(void)compileIn;

/**
 * 微信登录
 */
+ (void)login:(NSDictionary *)params;

/**
 * 微信登出
 */
+ (void)logout;

@end
