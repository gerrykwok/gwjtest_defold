//
//  NSString+ShareUtil.h
//  ykctest
//
//  Created by Xiejiajian on 2017/8/2.
//
//

#import <Foundation/Foundation.h>

@interface NSString (ShareUtil)

+ (NSString *)getEmojiWithUnicode:(NSString *)str;

//字典转Json C字符串
+ (const char *)dictionaryToCJsonString:(NSDictionary *)dict;

//字典转Json字符串
+ (NSString *)dictionaryToJson:(NSDictionary *)dict;

//日期转字符串 格式：yyyy/MM/dd HH:mm:ss
+ (NSString *)stringFromDateWithShareFormat:(NSDate *)date;

@end
