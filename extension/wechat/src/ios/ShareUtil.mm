#if defined(DM_PLATFORM_IOS)

#import <UIKit/UIKit.h>
#import "WeChatSDK1.8.3_NoPay/WXApi.h"
#import "ShareUtil.h"
#import "../wechat.h"

static int g_shareCallback;

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
}

+(NSString*)shareWithSDK:(NSDictionary*)params
{
	NSLog(@"gwjgwj,In ShareUtil.shareWithSDK,params=%@", params);

	NSString* shareType = params[@"type"];
	int scene = [params[@"scene"] intValue];
	NSString* text = params[@"text"];
	NSString* image = params[@"image"];
	NSString* title = params[@"title"];
	NSString* description = params[@"description"];
	NSString* url = params[@"url"];
	int callback = [params[@"callback"] intValue];
	g_shareCallback = callback;

	BOOL success = NO;

	if([shareType isEqualToString:@"text"])
	{
		SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
		req.bText = YES;
		req.text = text;
		req.scene = scene;
		success = [WXApi sendReq:req];
	}
	else if([shareType isEqualToString:@"image"])
	{
//		UIImage *image = [UIImage imageNamed:@"res2.png"];
//		imageData = UIImageJPEGRepresentation(image, 0.7);

		WXImageObject *imageObject = [WXImageObject object];
		imageObject.imageData = [NSData dataWithContentsOfFile:image];

		WXMediaMessage *message = [WXMediaMessage message];
//		NSString *filePath = [[NSBundle mainBundle] pathForResource:@"res5" ofType:@"jpg"];
//		message.thumbData = [NSData dataWithContentsOfFile:filePath];
		message.mediaObject = imageObject;

		SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
		req.bText = NO;
		req.message = message;
		req.scene = scene;
		success = [WXApi sendReq:req];
	}
	else if([shareType isEqualToString:@"music"])
	{
		WXMusicObject *musicObject = [WXMusicObject object];
		musicObject.musicUrl = url;
//		musicObject.musicLowBandUrl = musicObject.musicUrl;
//		musicObject.musicDataUrl = @"音乐数据url";
//		musicObject.musicLowBandDataUrl = musicObject.musicDataUrl;

		WXMediaMessage *message = [WXMediaMessage message];
		message.title = title;
		message.description = description;
		[message setThumbImage:[UIImage imageWithContentsOfFile:image]];
		message.mediaObject = musicObject;

		SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
		req.bText = NO;
		req.message = message;
		req.scene = scene;
		success = [WXApi sendReq:req];
	}
	else if([shareType isEqualToString:@"video"])
	{
		WXVideoObject *videoObject = [WXVideoObject object];
		videoObject.videoUrl = url;
//		videoObject.videoLowBandUrl = @"低分辨率视频url";

		WXMediaMessage *message = [WXMediaMessage message];
		message.title = title;
		message.description = description;
		[message setThumbImage:[UIImage imageWithContentsOfFile:image]];
		message.mediaObject = videoObject;

		SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
		req.bText = NO;
		req.message = message;
		req.scene = scene;
		success = [WXApi sendReq:req];
	}
	else if([shareType isEqualToString:@"webpage"])
	{
		WXWebpageObject *webpageObject = [WXWebpageObject object];
		webpageObject.webpageUrl = url;

		WXMediaMessage *message = [WXMediaMessage message];
		message.title = title;
		message.description = description;
		[message setThumbImage:[UIImage imageWithContentsOfFile:image]];
		message.mediaObject = webpageObject;

		SendMessageToWXReq *req = [[SendMessageToWXReq alloc] init];
		req.bText = NO;
		req.message = message;
		req.scene = scene;
		success = [WXApi sendReq:req];
	}
	else
	{
		return [NSString stringWithFormat:@"invalid share type:%@", shareType];
	}
	return success ? @"success" : @"failed";
}

+(void)notifyShareResult:(NSString*)res
{
	if(g_shareCallback > 0)
	{
		ext_invokeLuaCallbackWithString(g_shareCallback, [res UTF8String]);
		ext_unregisterLuaCallback(g_shareCallback);
		g_shareCallback = 0;
	}
}

@end

#endif
