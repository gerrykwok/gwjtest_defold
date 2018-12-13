#if defined(DM_PLATFORM_IOS)

#import "../xianliao.h"
#import "XianliaoSDK_iOS/SugramApiManager.h"
#import "XianliaoSDK_iOS/SugramApiObject.h"

@implementation XianliaoUtil

+(void)initXianliao:(NSString*)appid
{
	[SugramApiManager registerApp:appid];
}

+(void)login:(NSDictionary *)params
{
}

+(void)share:(NSDictionary *)params
{
	NSString *type = params[@"type"];
	int callback = [params[@"callback"] intValue];
}

@end

#endif
