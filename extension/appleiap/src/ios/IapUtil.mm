#if defined(DM_PLATFORM_IOS)

#import "IapUtil.h"
#import "../appleiap.h"
#include <extension/crypto/src/crypto.h>

@implementation IapUtil

+(void)static_init:(NSDictionary*)params
{
	[[IapUtil getInstance] init:params];
}

+(void)static_purgeInstance:(NSDictionary*)params
{
	[IapUtil purgeInstance];
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

+(void)purgeInstance
{
	if(g_instance)
	{
		[g_instance release];
		g_instance = nil;
	}
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
		m_transactionCallback = 0;
	}
	return self;
}

-(void)dealloc
{
	[super dealloc];
	[[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}

-(void)init:(NSDictionary*)params
{
	if(m_transactionCallback > 0)
	{
		ext_unregisterLuaCallback(m_transactionCallback);
		m_transactionCallback = 0;
	}
	m_transactionCallback = [params[@"callback"] intValue];
}

-(NSString*)canMakePurchases:(NSDictionary*)params
{
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
	//NSLog(@"all product id:%@", allPid);
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
	NSLog(@"request product failed,code=%ld,%@", (long)error.code, error);
	NSString *res = [NSString stringWithFormat:@"{\"success\":false,\"errCode\":%ld}", (long)error.code];
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
	//NSLog(@"updated transactions:%@", transactions);
	for(SKPaymentTransaction *transaction : transactions)
	{
		SKPaymentTransactionState state = transaction.transactionState;
		NSLog(@"transaction state:%ld", (long)state);
		switch(transaction.transactionState)
		{
		case SKPaymentTransactionStatePurchasing:
			break;
		case SKPaymentTransactionStateDeferred:
			break;
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
	NSData *receipt = nil;
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
		//receipt都有新旧的格式之分, https://wo1fsea.github.io/2014/08/17/About_IAP_Receipt/
		NSLog(@"NSFoundationVersionNumber=%f", NSFoundationVersionNumber);
		if (floor(NSFoundationVersionNumber) <= NSFoundationVersionNumber_iOS_6_1) {// Load resources for iOS 6.1 or earlier
			receipt = transaction.transactionReceipt;
		} else {
			NSURL *receiptURL = [[NSBundle mainBundle] appStoreReceiptURL];
			receipt = [NSData dataWithContentsOfURL:receiptURL];
		}
		receiptDataLength = receipt.length;
		receiptData = receipt.bytes;
		break;
	case SKPaymentTransactionStatePurchasing:
		ccstate = CCStorePaymentTransactionStatePurchasing;
		break;
	case SKPaymentTransactionStateRestored:
		ccstate = CCStorePaymentTransactionStateRestored;
	case SKPaymentTransactionStateDeferred:
		break;
	}

	NSMutableString *res = [NSMutableString stringWithCapacity:100];

	[res appendString:@"{"];

	[res appendFormat:@"\"state\":\"%s\",", CCStorePaymentTransactionStateNames[ccstate]];
	[res appendFormat:@"\"transactionIdentifier\":\"%@\",", transaction.transactionIdentifier];
	[res appendFormat:@"\"productIdentifier\":\"%@\",", transaction.payment.productIdentifier];
	[res appendFormat:@"\"quantity\":%ld,", (long)transaction.payment.quantity];
	[res appendFormat:@"\"date\":%g,", [transaction.transactionDate timeIntervalSince1970]];
	[res appendFormat:@"\"errorCode\":%d,", errorCode];
	[res appendFormat:@"\"errorString\":\"%s\"", errorDescription ? errorDescription : ""];

	if(receiptDataLength > 0)
	{
		// http://stackoverflow.com/questions/11242667/how-to-parse-apples-iap-receipt-mal-formatted-json
		char* receiptBase64 = crypto_encode_base64(receiptData, receiptDataLength);
		if(receiptBase64)
		{
			[res appendFormat:@",\"receipt_base64\":\"%s\"", receiptBase64];
			crypto_delete_buffer(receiptBase64);
		}
	}

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
