#if defined(DM_PLATFORM_IOS)

#include <dmsdk/sdk.h>
#include "luajavaoc_ios.h"
#include "../luacallback.h"

std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok)
{
	std::string sReturn;
	*ok = true;
	Class targetClass = NSClassFromString([NSString stringWithUTF8String:clazz]);
	if(!targetClass)
	{
		*ok = false;
		std::string str = (std::string)"failed to find class " + clazz;
		dmLogError("%s", str.c_str());
		return str;
	}
	SEL methodSel;
	NSString *methodName = [NSString stringWithFormat:@"%@:", [NSString stringWithUTF8String:method]];
	methodSel = NSSelectorFromString(methodName);
	if(methodSel == (SEL)0)
	{
		*ok = false;
		std::string str = (std::string)"failed to find method " + clazz + "." + method;
		dmLogError("%s", str.c_str());
		return str;
	}
	NSMethodSignature *methodSig = [targetClass methodSignatureForSelector:(SEL)methodSel];
	if(methodSig == nil)
	{
		*ok = false;
		std::string str = (std::string)"failed to get method signature of " + clazz + "." + method;
		dmLogError("%s", str.c_str());
		return str;
	}
	@try {
		NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSig];
		[invocation setTarget:targetClass];
		[invocation setSelector:methodSel];
		const char *returnType = [methodSig methodReturnType];
		NSError *err = nil;
		NSData *jsonData = [[NSString stringWithUTF8String:params] dataUsingEncoding:NSUTF8StringEncoding];
		NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
		if(err)
		{
			NSLog(@"parse json error:%@", err);
			NSLog(@"parsing json:%s", params);
		}
		else
		{
			[invocation setArgument:&dic atIndex:2];
		}
		[invocation invoke];
		NSString *sRet;
		if (strcmp(returnType, "@") == 0)//NSString or something derived from NSObject
		{
			id ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%@", ret];
		}
		else if (strcmp(returnType, "B") == 0) // BOOL
		{
			BOOL ret;
			[invocation getReturnValue:&ret];
			sRet = ret ? @"true" : @"false";
		}
		else if (strcmp(returnType, "c") == 0) // char
		{
			char ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%c", ret];
		}
		else if (strcmp(returnType, "i") == 0) // int
		{
			int ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%d", ret];
		}
		else if (strcmp(returnType, "f") == 0) // float
		{
			float ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%g", ret];
		}
		else
		{
			sRet = @"";
		}
		sReturn = [sRet UTF8String];
		return sReturn;
	}
	@catch (NSException *exception)
	{
		*ok = false;
		NSString *err = [NSString stringWithFormat:@"exception while invoke method %s.%s : %@", clazz, method, exception];
		dmLogError("%s", [err UTF8String]);
		return [err UTF8String];
	}
}

NSDictionary* ext_NSDictionaryFromLuaTable(lua_State *L, int index)
{
	NSMutableDictionary *dict = [NSMutableDictionary dictionary];
	NSDictionary *sub;
	int top = lua_gettop(L);
	NSString *key;
	std::string sValue;
	int i;
	char buf[256];
	lua_Number fValue;
	int funcId;

	int type = top > 0 ? lua_type(L, index) : LUA_TNIL;
//	dmLogInfo("gwjgwj,type in index is %d top=%d", type, top);
	if(type != LUA_TTABLE)
	{
		return dict;
	}

	lua_pushnil(L);  /* first key */
	while(lua_next(L, index-1) != 0)
	{
		/* 'key' is at index -2 and 'value' at index -1 */
		switch(lua_type(L, -2))
		{
		case LUA_TNIL:
			key = @"nil";
			break;
		case LUA_TBOOLEAN:
			key = lua_toboolean(L, -2) ? @"true" : @"false";
			break;
		case LUA_TNUMBER:
			key = [NSString stringWithFormat:@"%d", (int)lua_tonumber(L, -2)];
			break;
		case LUA_TSTRING:
			key = [NSString stringWithUTF8String:lua_tostring(L, -2)];
			break;
		default:
			key = nil;
			break;
		}

		if(key) switch(lua_type(L, -1))
		{
		case LUA_TBOOLEAN:
			[dict setObject:[NSNumber numberWithBool:lua_toboolean(L, -1)] forKey:key];
			break;
		case LUA_TNUMBER:
			[dict setObject:[NSNumber numberWithDouble:lua_tonumber(L, -1)] forKey:key];
			break;
		case LUA_TSTRING:
			[dict setObject:[NSString stringWithUTF8String:lua_tostring(L, -1)] forKey:key];
			break;
		case LUA_TFUNCTION:
			funcId = ext_registerLuaCallback(L, -1);
			[dict setObject:[NSNumber numberWithInt:funcId] forKey:key];
			break;
		case LUA_TTABLE:
			sub = ext_NSDictionaryFromLuaTable(L, -1);
			[dict setObject:sub forKey:key];
			break;
		default: break;
		}

//		dmLogInfo("%s - %s", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
//		dmLogInfo("\"%s\":%s", sKey.c_str(), sValue.c_str());

		lua_pop(L, 1);  /* removes 'value'; keeps 'key' for next iteration */
	}

	assert(top + 0 == lua_gettop(L));
	return dict;
}

std::string ext_callNativeStaticMethodBase(const char *clazz, const char *method, lua_State *L, int idxParam, bool *ok)
{
	std::string sReturn;
	*ok = true;
	Class targetClass = NSClassFromString([NSString stringWithUTF8String:clazz]);
	if(!targetClass)
	{
		*ok = false;
		std::string str = (std::string)"failed to find class " + clazz;
		dmLogError("%s", str.c_str());
		return str;
	}
	SEL methodSel;
	NSString *methodName = [NSString stringWithFormat:@"%@:", [NSString stringWithUTF8String:method]];
	methodSel = NSSelectorFromString(methodName);
	if(methodSel == (SEL)0)
	{
		*ok = false;
		std::string str = (std::string)"failed to find method " + clazz + "." + method;
		dmLogError("%s", str.c_str());
		return str;
	}
	NSMethodSignature *methodSig = [targetClass methodSignatureForSelector:(SEL)methodSel];
	if(methodSig == nil)
	{
		*ok = false;
		std::string str = (std::string)"failed to get method signature of " + clazz + "." + method;
		dmLogError("%s", str.c_str());
		return str;
	}
	@try {
		NSInvocation *invocation = [NSInvocation invocationWithMethodSignature:methodSig];
		[invocation setTarget:targetClass];
		[invocation setSelector:methodSel];
		const char *returnType = [methodSig methodReturnType];
		NSDictionary *dic = ext_NSDictionaryFromLuaTable(L, idxParam);
		[invocation setArgument:&dic atIndex:2];
		[invocation invoke];
		NSString *sRet;
		if (strcmp(returnType, "@") == 0)//NSString or something derived from NSObject
		{
			id ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%@", ret];
		}
		else if (strcmp(returnType, "B") == 0) // BOOL
		{
			BOOL ret;
			[invocation getReturnValue:&ret];
			sRet = ret ? @"true" : @"false";
		}
		else if (strcmp(returnType, "c") == 0) // char
		{
			char ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%c", ret];
		}
		else if (strcmp(returnType, "i") == 0) // int
		{
			int ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%d", ret];
		}
		else if (strcmp(returnType, "f") == 0) // float
		{
			float ret;
			[invocation getReturnValue:&ret];
			sRet = [NSString stringWithFormat:@"%g", ret];
		}
		else
		{
			sRet = @"";
		}
		sReturn = [sRet UTF8String];
		return sReturn;
	}
	@catch (NSException *exception)
	{
		*ok = false;
		NSString *err = [NSString stringWithFormat:@"exception while invoke method %s.%s : %@", clazz, method, exception];
		dmLogError("%s", [err UTF8String]);
		return [err UTF8String];
	}
}

#endif
