#import <UIKit/UIKit.h>
#import "WeChatSDK1.8.3_NoPay/WXApi.h"

@interface AppControllerWechat : NSObject <UIApplicationDelegate, WXApiDelegate>
{
	NSString *m_appid;
}

-(void)setWxAppId:(NSString*)appid;

@end
