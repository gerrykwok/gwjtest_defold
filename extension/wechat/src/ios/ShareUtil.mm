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
	NSString *UIActivityTypeWeixin = @"com.tencent.xin.sharetimeline";
	NSString *title = params[@"title"];
	NSString *image = params[@"image"];
	NSString *content = params[@"content"];
	NSString *urlStr = params[@"url"];
	int callback = [params[@"callback"] intValue];//回调
	NSLog(@"gwjgwj,callback=%d", callback);
	UIImage *img = nil;
	if(image) img = [UIImage imageWithContentsOfFile:image];

	NSMutableArray *items = [[NSMutableArray alloc] init];
	if(title && [title length] > 0) [items addObject:title];
	if(img) [items addObject:img];
	if(content && [content length] > 0) [items addObject:content];//加入这个会导致分享到微信时:获取数据异常
	if(urlStr && [urlStr length] > 0)
	{
		NSURL *url = [NSURL URLWithString:urlStr];
		if(url == nil) url = [NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
		[items addObject:url];
	}

	UIActivityViewController *activityController = [[UIActivityViewController alloc] initWithActivityItems:items applicationActivities:nil];
	//不出现在活动项目
/*	activityController.excludedActivityTypes = @[
		UIActivityTypePostToFacebook,UIActivityTypePostToTwitter,UIActivityTypePostToWeibo,UIActivityTypeMessage,UIActivityTypeMail,
		UIActivityTypePrint,UIActivityTypeCopyToPasteboard,UIActivityTypeAssignToContact,UIActivityTypeSaveToCameraRoll,
		UIActivityTypeAddToReadingList,UIActivityTypePostToFlickr,UIActivityTypePostToVimeo,UIActivityTypePostToTencentWeibo,
		UIActivityTypeAirDrop,UIActivityTypeOpenInIBooks,
		@"com.apple.mobilenotes.SharingExtension",@"com.apple.mobileslideshow.StreamShareService",
		@"com.tencent.mqq.ShareExtension",//QQ
		@"com.apple.NanoTimeKitCompanion.CreateWatchFace",//创建表盘
		@"com.apple.CloudDocsUI.AddToiCloudDrive",//存储到文件
		@"com.alipay.iphoneclient.ExtensionSchemeShare",//支付宝
		@"com.apple.reminders.RemindersEditorExtension",//提醒事项
		];
		*/
	activityController.completionWithItemsHandler = ^(UIActivityType  _Nullable activityType, BOOL completed, NSArray * _Nullable returnedItems, NSError * _Nullable activityError) {
		NSLog(@"activityType:%@,completed:%d,error:%@", activityType, completed, activityError);
		NSString *res;
		if(completed)
		{
			if(!activityError)//success
			{
				if(activityType == nil)
					res = @"{\"result\":0}";
				else if([activityType compare:UIActivityTypeWeixin] == NSOrderedSame)
					res = @"{\"result\":0}";
					else res = @"{\"result\":-1}";
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
		ext_invokeLuaCallbackWithString(callback, [res UTF8String]);
		ext_unregisterLuaCallback(callback);
	};

	UIViewController *viewController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
	if([activityController respondsToSelector:@selector(popoverPresentationController)])
		activityController.popoverPresentationController.sourceView = [viewController view];//加入这句为了解决在ipad上崩溃, 参考:http://www.cocoachina.com/bbs/read.php?tid=233799
	[viewController presentViewController:activityController animated:YES completion:nil];//这句在iphone7上可以
}

@end

#endif
