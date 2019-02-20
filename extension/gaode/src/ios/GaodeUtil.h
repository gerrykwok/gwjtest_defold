#import <Foundation/Foundation.h>

@interface GaodeUtil : NSObject

+(void)locationInit:(NSDictionary*)dict;
+(void)locationStart:(NSDictionary*)dict;
+(void)locationStop:(NSDictionary*)dict;
+(int)getLocationAuthorizationStatus:(NSDictionary*)dict;//0=没有，1=还没询问状态，2=已经获得授权
+(void)jumpLocationSetting:(NSDictionary*)dict;

@end
