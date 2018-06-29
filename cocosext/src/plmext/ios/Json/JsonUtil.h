//
//  JsonUtil.h
//  gdmj
//
//  Created by dev on 16/5/10.
//
//

#ifndef JsonUtil_h
#define JsonUtil_h


#import <Foundation/Foundation.h>
#include <string>

using namespace std;

@interface JsonUtil : NSObject
{
}

+(string) buildJsonString:(NSMutableDictionary *)dict;

+(string) buildJsonKey:(NSString *)key stringValue:(NSString *)value;

+(string) buildJsonKey:(NSString *)key intValue:(int)value;

+(string) buildJsonKey:(NSString *)key;

+(string) buildStringValue:(NSString *)value;

+(string) buildIntValue:(int)value;

/////////////////////////////////////////////

+(std::string) json_key:(std::string)str;

+(std::string) json_int_value:(int)value;

+(std::string) json_string_value:(std::string)str;

// 对象
+(void) json_object_begin:(std::string &)str;

+(void) json_object_end:(std::string &)str;

+(void) json_object_append_string_item:(std::string &)str key:(const char *)key value:(const char *)value;

+(void) json_object_append_number_item:(std::string &)str key:(const char *)key value:(int)value;

+(void) json_object_append_array_item:(std::string &)str key:(const char *)key value:(const char *)value;

// 数组
+(void) json_array_begin:(std::string &)str;

+(void) json_array_end:(std::string &)str;

@end

#endif /* JsonUtil_h */
