#if defined(DM_PLATFORM_IOS)

#import "CrasheyeUtil.h"
#include "crasheye_ios_v2.6.3/Crasheye.h"

@implementation CrasheyeUtil

+(void)sendScriptError:(NSDictionary*)params
{
	NSString *title = params[@"title"];
	NSString *content = params[@"content"];
//	NSLog(@"gwjgwj,send script error,title=%@,content=%@", title, content);
	[Crasheye sendScriptExceptionRequestWithTitle:title exception:content file:nil language:@"lua"];
}

+(void)setUserId:(NSDictionary*)params
{
	NSString *userId = params[@"userId"];
//	NSLog(@"gwjgwj,set user id:%@", userId);
	[Crasheye setUserID:userId];
}

+(void)addExtraData:(NSDictionary*)params
{
	int i;
	NSArray *allkey = [params allKeys];
	for(i = 0; i < [allkey count]; ++i)
	{
		NSString *key = [allkey objectAtIndex:i];
		NSString *value = [params objectForKey:key];
//		NSLog(@"gwjgwj,addExtra,key=%@,value=%@", key, value);
		[Crasheye addExtraDataWithKey:key withValue:value];
	}
}

+(void)removeExtraData:(NSDictionary*)params
{
	NSString *key = params[@"key"];
	if(key)
	{
		NSLog(@"crasheye:remove extra of key %@", key);
		[Crasheye removeExtraDataWithKey:key];
	}
}

+(void)clearExtraData:(NSDictionary*)params
{
	NSLog(@"crasheye:clear all extra data");
	[Crasheye clearExtraData];
}

@end

#endif
