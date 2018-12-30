#if defined(DM_PLATFORM_IOS)

#import "IapUtil.h"
#import "../appleiap.h"

@implementation IapUtil

+(void)static_init:(NSDictionary*)params
{
	[[IapUtil getInstance] init:params];
}

+(void)static_loadProducts:(NSDictionary*)params
{
	[[IapUtil getInstance] loadProducts:params];
}

+(NSString*)static_purchase:(NSDictionary*)params
{
	return [[IapUtil getInstance] purchase:params];
}

+(void)static_finishTransaction:(NSDictionary*)params
{
	[[IapUtil getInstance] finishTransaction:params];
}

static IapUtil* g_instance;
+(IapUtil*)getInstance
{
	if(!g_instance)
	g_instance = [[IapUtil alloc] init];
	return g_instance;
}

-(void)init:(NSDictionary*)params
{
}

-(void)loadProducts:(NSDictionary*)params
{
	NSDictionary *productsId = params[@"productsId"];
	int callback = [params[@"callback"] intValue];
	m_loadProductCallback = callback;
	NSMutableSet *allPid = [NSMutableSet set];
	NSArray *keys = [productsId allKeys];
	for(NSString *key : keys)
	{
		NSString *id = productsId[key];
		[allPid addObject:id];
	}
	NSLog(@"all product id:%@", allPid);
	SKProductsRequest *request = [[SKProductsRequest alloc] initWithProductIdentifiers:allPid];
	request.delegate = self;
	[request autorelease];
	[request start];
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response
{
	NSMutableString *res = [NSMutableString stringWithCapacity:100];
	[res appendString:@"{\"success\":true,"];
	[res appendString:@"\"products\":["];
	for (int i = 0; i < response.products.count; ++i)
	{
		SKProduct* product = [response.products objectAtIndex:i];
		NSLog(@"product:id=%@,localizedTitle=%@,localizeDesc=%@,price=%f,localeId=%@", 
			product.productIdentifier,
			product.localizedTitle,
			product.localizedDescription,
			[product.price floatValue],
			product.priceLocale.localeIdentifier);
		if(i > 0) [res appendFormat:@","];
		[res appendFormat:@"{\"productIdentifier\":\"%@\",\"localizedTitle\":\"%@\",\"localizedDescription\":\"%@\",\"priceLocale\":\"%@\",\"price\":%@}",
			product.productIdentifier,
			product.localizedTitle,
			product.localizedDescription,
			product.priceLocale.localeIdentifier,
			product.price];
	}
	[res appendString:@"]"];
	NSLog(@"invalid product id:");
	[res appendString:@",\"invalidProductsId\":["];
	for (int i = 0; i < response.invalidProductIdentifiers.count; ++i)
	{
		NSString* productId = [response.invalidProductIdentifiers objectAtIndex:i];
		NSLog(@"    %@", productId);
		[res appendFormat:@"\"%@\"", productId];
	}
	[res appendString:@"]"];
	[res appendString:@"}"];
	[self notifyLuaOfRequestProduct:res];
}

- (void)request:(SKRequest *)request didFailWithError:(NSError *)error
{
	NSLog(@"request product failed,code=%d,%@", error.code, error);
	NSString *res = [NSString stringWithFormat:@"{\"success\":false,\"errCode\":%d}", error.code];
	[self notifyLuaOfRequestProduct:res];
}

-(void)notifyLuaOfRequestProduct:(NSString*)res
{
	if(m_loadProductCallback > 0)
	{
		ext_invokeLuaCallbackWithString(m_loadProductCallback, [res UTF8String]);
		ext_unregisterLuaCallback(m_loadProductCallback);
		m_loadProductCallback = 0;
	}
}

-(NSString*)purchase:(NSDictionary*)params
{
	return @"success";
}

-(void)finishTransaction:(NSDictionary*)params
{
}

@end

#endif
