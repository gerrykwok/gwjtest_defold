//
//  JsonUtil.m
//  gdmj
//
//  Created by dev on 16/5/10.
//
//

#import "JsonUtil.h"

// json相关

// 转义引号
#define ESCAPE                      "\""
// 左括号
#define LEFT_BRACE                  "{"
// 右括号
#define RIGHT_BRACE                 "}"
// 参数分隔符
#define SEPERATOR_STRING            ","
// 冒号
#define COLON                       ":"

// json相关

@implementation JsonUtil


+(string) buildJsonString:(NSMutableDictionary *)dict
{
    NSUInteger size = [dict count];
    NSUInteger idx = 0;
    
    string res;
    
    res.append(LEFT_BRACE);
    for (NSString *key in dict) {
        res.append([JsonUtil buildJsonKey:key stringValue:[dict objectForKey:key]]);
        
        if(idx == size - 1)
        {
            
        }
        else
        {
            res.append(SEPERATOR_STRING);
        }
        
        idx++;
    }
    res.append(RIGHT_BRACE);
    
    NSLog(@"response json str:%@", [NSString stringWithUTF8String:res.c_str()]);
    
    return res;
}

+(string) buildJsonKey:(NSString *)key stringValue:(NSString *)value
{
    string res;
    res.append([JsonUtil buildJsonKey:key]);
    res.append(COLON);
    res.append([JsonUtil buildStringValue:value]);
    
    return res;
}

+(string) buildJsonKey:(NSString *)key intValue:(int)value
{
    string res;
    res.append([JsonUtil buildJsonKey:key]);
    res.append(COLON);
    res.append([JsonUtil buildIntValue:value]);
    
    return res;
}

+(string) buildJsonKey:(NSString *)key
{
    string res;
    res.append(ESCAPE);
    res.append( [key cStringUsingEncoding:NSUTF8StringEncoding] );
    res.append(ESCAPE);
    
    return res;
}

+(string) buildStringValue:(NSString *)value
{
    string res;
    res.append(ESCAPE);
    res.append( [value cStringUsingEncoding:NSUTF8StringEncoding] );
    res.append(ESCAPE);
    return res;
}

+(string) buildIntValue:(int)value
{
    string res;
    res.append( [[NSString stringWithFormat:@"%d", value] cStringUsingEncoding:NSUTF8StringEncoding] );
    return res;
}

//////////////////////////////////////////////////////////////////////////////////////////////
+(std::string) json_key:(std::string)str
{
    std::string json_str;
    json_str.append("\"");
    json_str.append(str);
    json_str.append("\"");
    return json_str;
}

+(std::string) json_int_value:(int)value
{
    std::string json_str;
    char tempBuf[64];
    sprintf(tempBuf, "%d", value);
    json_str.assign(tempBuf);
    return json_str;
}

+(std::string) json_string_value:(std::string)str
{
    std::string json_str;
    json_str.append("\"");
    json_str.append(str);
    json_str.append("\"");
    return json_str;
}

+(void) json_object_begin:(std::string &)str;
{
    str.assign("{");
}

+(void) json_object_end:(std::string &)str
{
    str.append("}");
}

+(void) json_object_append_string_item:(std::string &)str key:(const char *)key value:(const char *)value
{
    const char *pBuf = str.c_str();
    size_t len = str.length();
    
    if('{' != pBuf[len - 1])
    {
        str.append(",");
    }
    
    str.append([JsonUtil json_key:key]);
    str.append(":");
    str.append([JsonUtil json_string_value:value]);
}

+(void) json_object_append_number_item:(std::string &)str key:(const char *)key value:(int)value
{
    const char *pBuf = str.c_str();
    size_t len = str.length();
    
    if('{' != pBuf[len - 1])
    {
        str.append(",");
    }
    
    str.append([JsonUtil json_key:key]);
    str.append(":");
    str.append([JsonUtil json_int_value:value]);
}

+(void) json_object_append_array_item:(std::string &)str key:(const char *)key value:(const char *)value
{
    const char *pBuf = str.c_str();
    size_t len = str.length();
    
    if('{' != pBuf[len - 1])
    {
        str.append(",");
    }
    
    str.append([JsonUtil json_key:key]);
    str.append(":");
    str.append(value);
}

//+(void) json_object_append_array_item:(std::string &)str key:(const char *)key value:(const char *)value
//{
//    [JsonUtil json_object_append_string_item:str key:key value:value];
//}

//+(void) json_object_array_append_object:(std::string &)str value:(const char *)value
//{
//    const char *pBuf = str.c_str();
//    size_t len = str.length();
//    
//    if('[' != pBuf[len - 1])
//    {
//        str.append(",");
//    }
//    
//    str.append(value);
//}

//+(void) json_object_append_array_item:(std::string &)str key:(const char *)key array:(const char *[])array size:(int)size
//{
//    std::string object_array_json;
//    
//    [JsonUtil json_array_begin:object_array_json];
//    
//    for (int i = 0; i < size; i++)
//    {
//        [JsonUtil json_object_array_append_object:object_array_json value:array[i]];
//    }
//    
//    [JsonUtil json_array_end:object_array_json];
//    
//    [JsonUtil json_object_append_array_item:str key:key value:object_array_json.c_str()];
//}

+(void) json_array_begin:(std::string &)str
{
    str.assign("[");
}

+(void) json_array_end:(std::string &)str
{
    str.append("]");
}

@end