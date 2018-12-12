#if defined(DM_PLATFORM_IOS)

#import "MiscFunc.h"

@implementation MiscFunc

+(NSString*)canOpenApp:(NSDictionary*)params
{
	NSString *url = params[@"url"];
	if([[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:url]])
		return @"true";
	return @"false";
}

@end

#endif
