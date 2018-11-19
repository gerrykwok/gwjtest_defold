#if defined(DM_PLATFORM_IOS)

//
//  NSString+ShareUtil.m
//  ykctest
//
//  Created by Xiejiajian on 2017/8/2.
//
//

#import "NSString+ShareUtil.h"

@implementation NSString (ShareUtil)

+ (NSString *)getEmojiWithUnicode:(NSString *)str {
    unsigned long  unicodeIntValue= strtoul([str UTF8String], 0, 16);
    UTF32Char inputChar = (UTF32Char)unicodeIntValue;
    inputChar = NSSwapHostIntToLittle(inputChar);
    NSString *resultStr = [[NSString alloc] initWithBytes:&inputChar length:4 encoding:NSUTF32LittleEndianStringEncoding];
    return resultStr;
}

+ (const char *)dictionaryToCJsonString:(NSDictionary *)dict {
    NSString *jsonStr = [self dictionaryToJson:dict];
    const char *cJsonStr = [jsonStr UTF8String];
    return cJsonStr;
}

+ (NSString *)dictionaryToJson:(NSDictionary *)dict{
    NSError *error = nil;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&error];
    return [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
}

+ (NSString *)stringFromDateWithShareFormat:(NSDate *)date {
    return [self stringFromDate:date format:@"yyyy/MM/dd HH:mm:ss"];
}

+ (NSString *)stringFromDate:(NSDate *)date format:(NSString *)format {
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:format];
    return [dateFormatter stringFromDate:date];
}


@end

#endif
