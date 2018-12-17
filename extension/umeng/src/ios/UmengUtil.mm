#if defined(DM_PLATFORM_IOS)

#import <UMCommon/UMCommon.h>
#import <UMCommonLog/UMCommonLogManager.h>
#import <UMAnalytics/MobClick.h>
#include "../umeng.h"
#import "UmengUtil.h"

@implementation UmengUtil

+(void)init:(NSDictionary*)params
{
	NSString *appkey = params[@"appkey"];
	NSString *channel = params[@"channel"];
//	NSLog(@"gwjgwj,init umeng with appkey:%@ channel:%@", appkey, channel);
	[UMCommonLogManager setUpUMCommonLogManager];//开发者需要显式的调用此函数，日志系统才能工作
	[UMConfigure setLogEnabled:YES];
	[UMConfigure initWithAppkey:appkey channel:channel];
	[MobClick setScenarioType:E_UM_GAME];
}

+(void)sendEvent:(NSDictionary*)params
{
	NSString *eventName = params[@"name"];
	NSDictionary *args = params[@"args"];
//	NSLog(@"gwjgwj,eventname=%@,args=%@", eventName, args);
	if(args && [args count] > 0)
	{
		//发送到友盟的事件参数key-value必须都是String类型，故需要作转换
		NSMutableDictionary *attrib = [NSMutableDictionary dictionary];
		NSArray *keys = [args allKeys];
		for(NSString *key : keys)
		{
			NSString *value = [args[key] description];
			[attrib setObject:value forKey:key];
		}
//		NSLog(@"gwjgwj,send to umeng,attrib=%@", attrib);
		[MobClick event:eventName attributes:attrib];
	}
	else
	{
		[MobClick event:eventName];
	}
}

@end

#endif
