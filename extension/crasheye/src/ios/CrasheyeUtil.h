#import <Foundation/Foundation.h>

@interface CrasheyeUtil : NSObject

+(void)sendScriptError:(NSDictionary*)params;
+(void)setUserId:(NSDictionary*)params;
+(void)addExtraData:(NSDictionary*)params;
+(void)removeExtraData:(NSDictionary*)params;
+(void)clearExtraData:(NSDictionary*)params;

@end
