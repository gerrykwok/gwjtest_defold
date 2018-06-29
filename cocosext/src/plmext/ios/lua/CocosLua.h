//
//  CocosLua.h
//  gdmj
//
//  Created by dev on 16/5/10.
//
//

#ifndef CocosLua_h
#define CocosLua_h

#import <Foundation/Foundation.h>

@interface CocosLua : NSObject
{
}

/**
 * lua回调函数
 *
 **/
+(void) callLuaFunction:(int)funcId res:(const char *)str;

+(void) callGlocalLuaFunction:(const char*)funcName res:(const char *)str;

@end

#endif /* CocosLua_h */
