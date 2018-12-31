#if defined(DM_PLATFORM_IOS)

#import "IapUtil.h"
#import "../appleiap.h"

@implementation IapUtil

+(void)static_init:(NSDictionary*)params
{
	[[IapUtil getInstance] init:params];
}

+(NSString*)static_canMakePurchases:(NSDictionary*)params
{
	return [[IapUtil getInstance] canMakePurchases:params];
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

- (id)init
{
	self = [super init];
	if (self)
	{
		[[SKPaymentQueue defaultQueue] addTransactionObserver:self];
		m_loadedProducts = [NSMutableDictionary dictionaryWithCapacity:50];
		m_allTransactions = [NSMutableDictionary dictionaryWithCapacity:50];
		//[m_loadedProducts retain];
		//receiptVerifyMode_ = CCStoreReceiptVerifyModeNone;
	}
	return self;
}

-(void)init:(NSDictionary*)params
{
	NSLog(@"IapUtil init,params=%@", params);
	m_transactionCallback = [params[@"callback"] intValue];
}

-(NSString*)canMakePurchases:(NSDictionary*)params
{
	NSLog(@"gwjgwj oc,invoke canMakePurchases,params=%@", params);
	BOOL can = [SKPaymentQueue canMakePayments];
	return can ? @"yes" : @"no";
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
	[m_loadedProducts removeAllObjects];
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
		[m_loadedProducts setObject:product forKey:product.productIdentifier];
//		NSLog(@"product:id=%@,localizedTitle=%@,localizeDesc=%@,price=%f,localeId=%@", 
//			product.productIdentifier,
//			product.localizedTitle,
//			product.localizedDescription,
//			[product.price floatValue],
//			product.priceLocale.localeIdentifier);
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
	NSString *productId = params[@"productId"];
	if(!productId) return @"not specify productId";
	SKProduct *product = [m_loadedProducts objectForKey:productId];
	if(!product) return [NSString stringWithFormat:@"invalid productId:%@", productId];
	[[SKPaymentQueue defaultQueue] addPayment:[SKPayment paymentWithProduct:product]];
	return @"success";
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions
{
	NSLog(@"updated transactions:%@", transactions);
	for(SKPaymentTransaction *transaction : transactions)
	{
		SKPaymentTransactionState state = transaction.transactionState;
		NSLog(@"transaction state:%d", state);
//		if(state != SKPaymentTransactionStatePurchasing)
//			[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
		switch(transaction.transactionState)
		{
		case SKPaymentTransactionStatePurchased:
			[self transactionCompleted:transaction];
			break;
		case SKPaymentTransactionStateFailed:
			[self transactionFailed:transaction];
			break;
		case SKPaymentTransactionStateRestored:
			[self transactionRestored:transaction];
			break;
		}
	}
}

-(void)transactionCompleted:(SKPaymentTransaction *)transaction
{
	[m_allTransactions setObject:transaction forKey:transaction.transactionIdentifier];
	[self notifyLuaOfTransaction:transaction];
}

-(void)transactionFailed:(SKPaymentTransaction *)transaction
{
	[m_allTransactions setObject:transaction forKey:transaction.transactionIdentifier];
	[self notifyLuaOfTransaction:transaction];
}

-(void)transactionRestored:(SKPaymentTransaction *)transaction
{
	[m_allTransactions setObject:transaction forKey:transaction.transactionIdentifier];
	[self notifyLuaOfTransaction:transaction];
}

typedef enum {
	CCStorePaymentTransactionStateNull = 0,
	CCStorePaymentTransactionStatePurchasing = 1,
	CCStorePaymentTransactionStatePurchased,
	CCStorePaymentTransactionStateFailed,
	CCStorePaymentTransactionStateRestored,
	CCStorePaymentTransactionStateCancelled,
} CCStorePaymentTransactionState;
static const char* const CCStorePaymentTransactionStateNames[] = {
	"unknown",
	"purchasing",
	"purchased",
	"failed",
	"restored",
	"cancelled"
};

-(void)notifyLuaOfTransaction:(SKPaymentTransaction *)transaction
{
	if(m_transactionCallback <= 0) return;

	CCStorePaymentTransactionState ccstate = CCStorePaymentTransactionStateNull;
	int errorCode           = 0;
	int receiptDataLength   = 0;
	const void *receiptData = NULL;
	const char *errorDescription = NULL;
	switch(transaction.transactionState)
	{
	case SKPaymentTransactionStateFailed:
		errorCode = transaction.error.code;
		/**
		enum {
			SKErrorUnknown,
			SKErrorClientInvalid,       // client is not allowed to issue the request, etc.
			SKErrorPaymentCancelled,    // user cancelled the request, etc.
			SKErrorPaymentInvalid,      // purchase identifier was invalid, etc.
			SKErrorPaymentNotAllowed    // this device is not allowed to make the payment
		};
		*/
		if(errorCode == SKErrorPaymentCancelled)
		{
			ccstate = CCStorePaymentTransactionStateCancelled;
		}
		else
		{
			ccstate = CCStorePaymentTransactionStateFailed;
		}
		errorDescription = [transaction.error.localizedDescription UTF8String];
		break;
	case SKPaymentTransactionStatePurchased:
		ccstate = CCStorePaymentTransactionStatePurchased;
		receiptDataLength = transaction.transactionReceipt.length;
		receiptData = transaction.transactionReceipt.bytes;
		break;
	case SKPaymentTransactionStatePurchasing:
		ccstate = CCStorePaymentTransactionStatePurchasing;
		break;
	case SKPaymentTransactionStateRestored:
		ccstate = CCStorePaymentTransactionStateRestored;
	}

	NSMutableString *res = [NSMutableString stringWithCapacity:100];

	[res appendString:@"{"];

	[res appendFormat:@"\"state\":\"%s\",", CCStorePaymentTransactionStateNames[ccstate]];
	[res appendFormat:@"\"transactionIdentifier\":\"%@\",", transaction.transactionIdentifier];
	[res appendFormat:@"\"productIdentifier\":\"%@\",", transaction.payment.productIdentifier];
	[res appendFormat:@"\"quantity\":%d,", transaction.payment.quantity];
	[res appendFormat:@"\"date\":%g,", [transaction.transactionDate timeIntervalSince1970]];
	[res appendFormat:@"\"errorCode\":%d,", errorCode];
	[res appendFormat:@"\"errorString\":\"%s\"", errorDescription ? errorDescription : ""];

	[res appendString:@"}"];
	
	ext_invokeLuaCallbackWithString(m_transactionCallback, [res UTF8String]);
}

-(void)finishTransaction:(NSDictionary*)params
{
	NSString *transactionId = params[@"transactionId"];
	if(!transactionId)
	{
		NSLog(@"finishTransaction:must specify transaction id");
		return;
	}
	SKPaymentTransaction *transaction = [m_allTransactions objectForKey:transactionId];
	if(!transaction)
	{
		NSLog(@"finishTransaction:invalid transaction id:%@", transactionId);
		return;
	}
	[[SKPaymentQueue defaultQueue] finishTransaction:transaction];
	[m_allTransactions removeObjectForKey:transactionId];
}

@end

#endif
