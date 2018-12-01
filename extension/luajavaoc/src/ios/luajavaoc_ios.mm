#if defined(DM_PLATFORM_IOS)

#include <dmsdk/sdk.h>
#include "luajavaoc_ios.h"
#include "../luacallback.h"

NSDictionary* ext_jsonToDict(const char *jsonString)
{
	if (jsonString == NULL) {
		return nil;
	}

	NSData *jsonData = [NSData dataWithBytesNoCopy:(void*)jsonString length:strlen(jsonString)+1];
	NSError *err;
	NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&err];
	if(err)
	{
		NSLog(@"parse json error: %@", err);
		return nil;
	}
	return dic;
}

std::string ext_callOcStaticMethod(const char *clazz, const char *method, const char *params, bool *ok)
{
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
		NSUInteger returnLength = [methodSig methodReturnLength];
		const char *returnType = [methodSig methodReturnType];
		dmLogInfo("gwjgwj,returnLength=%ld,returnType=%s,params=%s", (unsigned long)returnLength, returnType, params);
		NSDictionary* dic = ext_jsonToDict(params);
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
		return [sRet UTF8String];
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
