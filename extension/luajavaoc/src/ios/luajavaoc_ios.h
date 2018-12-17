#pragma once

#import <Foundation/Foundation.h>

NSDictionary* ext_NSDictionaryFromLuaTable(lua_State *L, int index);

@interface NSBoolean : NSObject
{
	BOOL m_b;
}

+(NSBoolean*)fromBool:(BOOL)b;
-(id)initWithBool:(BOOL)b;
-(BOOL)boolValue;
//override
-(NSString*)description;

@end
