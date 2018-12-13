#import <Foundation/Foundation.h>

@interface XianliaoUtil : NSObject

+(void)initXianliao:(NSString*)appid;
+(void)login:(NSDictionary *)params;
+(void)share:(NSDictionary *)params;

@end
