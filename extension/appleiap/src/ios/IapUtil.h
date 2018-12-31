#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface IapUtil : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver>
{
	int m_loadProductCallback;
	NSMutableDictionary *m_loadedProducts;
	NSMutableDictionary *m_allTransactions;
	int m_transactionCallback;
}

//SKProductsRequestDelegate
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error;

//SKPaymentTransactionObserver
- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions;

-(void)notifyLuaOfRequestProduct:(NSString*)res;

@end
