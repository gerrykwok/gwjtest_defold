#if defined(DM_PLATFORM_IOS)

#import "IapUtil.h"
#import "../appleiap.h"

@implementation IapUtil

+(void)static_init:(NSDictionary*)params
{
}

+(void)static_loadProducts:(NSDictionary*)params
{
	NSDictionary *productsId = params[@"productsId"];
	int callback = [params[@"callback"] intValue];
	NSLog(@"productsId=%@", productsId);
}

+(NSString*)static_purchase:(NSDictionary*)params
{
	return @"success";
}

+(void)static_finishTransaction:(NSDictionary*)params
{
}

@end

#endif
