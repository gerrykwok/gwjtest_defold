#import <Foundation/Foundation.h>
#import <StoreKit/StoreKit.h>

@interface IapUtil : NSObject <SKProductsRequestDelegate>
{
	int m_loadProductCallback;
}

//SKProductsRequestDelegate
- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response;
- (void)request:(SKRequest *)request didFailWithError:(NSError *)error;

-(void)notifyLuaOfRequestProduct:(NSString*)res;

@end
