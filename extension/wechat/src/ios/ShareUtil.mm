#if defined(DM_PLATFORM_IOS)

#import <UIKit/UIKit.h>
#import "ShareUtil.h"
#import "../wechat.h"

@implementation ShareUtil

+(void)shareWithSystem:(NSDictionary*)params
{
	//参考: https://www.jianshu.com/p/0816a30cf255
	//https://www.jianshu.com/p/9de528d96466
	NSLog(@"gwjgwj,In ShareUtil.shareWithSystem,params=%@", params);
//	NSString *UIActivityTypeWeixin = @"com.tencent.xin.sharetimeline";
	NSString *title = params[@"title"];
	NSString *image = params[@"image"];
	NSString *content = params[@"content"];
	NSString *urlStr = params[@"url"];
	int callback = [params[@"callback"] intValue];//回调
	NSDictionary* exclude = params[@"excludeActivityType"];
	NSLog(@"gwjgwj,callback=%d,image=%@", callback, image);

	/*

	NSMutableArray *items = [[NSMutableArray alloc] init];
	if(title && [title length] > 0)
	{
		NSLog(@"gwjgwj,add title:%@", title);
		[items addObject:title];
	}
	if(image && [image length] > 0)
	{
		NSLog(@"gwjgwj,add image:%@", image);
		UIImage *img = [UIImage imageWithContentsOfFile:image];
		NSLog(@"gwjgwj,img=%@", img);
		[items addObject:img];
	}
	if(content && [content length] > 0)
	{
		NSLog(@"gwjgwj,add content:%@", content);
		[items addObject:content];//加入这个会导致分享到微信时:获取数据异常
	}
	if(urlStr && [urlStr length] > 0)
	{
		NSLog(@"gwjgwj,add url:%@", urlStr);
		NSURL *url = [NSURL URLWithString:urlStr];
		if(url == nil) url = [NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
		[items addObject:url];
	}

	UIActivityViewController *activityController = [[UIActivityViewController alloc] initWithActivityItems:items applicationActivities:nil];
	//不出现在活动项目
	if(exclude)
	{
		NSMutableArray *excludeActivityTypes = [[NSMutableArray alloc] init];
		NSArray *allkeys = [exclude allKeys];
		for(int i = 0; i < allkeys.count; ++i)
		{
			NSString *key = [allkeys objectAtIndex:i];
			NSString *value = [exclude objectForKey:key];
//			NSLog(@"key:%@,value:%@", key, value);
			[excludeActivityTypes addObject:value];
		}
		if([excludeActivityTypes count] > 0)
		{
			activityController.excludedActivityTypes = excludeActivityTypes;
		}
	}
	activityController.completionWithItemsHandler = ^(UIActivityType  _Nullable activityType, BOOL completed, NSArray * _Nullable returnedItems, NSError * _Nullable activityError) {
		NSLog(@"activityType:%@,completed:%d,error:%@", activityType, completed, activityError);
		NSString *res;
		if(completed)
		{
			if(!activityError)//success
			{
				res = [NSString stringWithFormat:@"{\"result\":0, \"activity_type\":\"%@\"}", activityType];
			}
			else//error
			{
				res = @"{\"result\":-1}";
			}
		}
		else//canceled
		{
			res = @"{\"result\":-2}";
		}
		if(callback > 0)
		{
			ext_invokeLuaCallbackWithString(callback, [res UTF8String]);
			ext_unregisterLuaCallback(callback);
		}
	};

	UIViewController *viewController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
	if([activityController respondsToSelector:@selector(popoverPresentationController)])
		activityController.popoverPresentationController.sourceView = [viewController view];//加入这句为了解决在ipad上崩溃, 参考:http://www.cocoachina.com/bbs/read.php?tid=233799
	[viewController presentViewController:activityController animated:YES completion:nil];//这句在iphone7上可以
	*/
}

@end

#endif
