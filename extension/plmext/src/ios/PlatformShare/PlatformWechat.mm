#if defined(DM_PLATFORM_IOS)

#import "PlatformWechat.h"
#import <ShareSDK/ShareSDK.h>
#import <ShareSDKExtension/ShareSDK+Extension.h>
#import "../lua/CocosLua.h"
#import "NSString+ShareUtil.h"
#import "ShareData.h"
#import "ShareSdkConstant.h"


NSString *WEICHAT_UID_PREFIX = @"wechat_";

@implementation PlatformWechat

+ (void)login:(NSDictionary *)params {
    [ShareSDK getUserInfo:SSDKPlatformTypeWechat
           onStateChanged:^(SSDKResponseState state, SSDKUser *user, NSError *error) {
               int callback = [ShareData handleShareCallback:params];
               NSLog(@"state %lu", (unsigned long)state);
               NSDictionary *dict = [NSDictionary dictionary];
               if (state == SSDKResponseStateSuccess) {
                   dict = @{@"result": @(0),
                            @"userInfo":@{
                                    //用户信息:
                                    @"uid": [NSString stringWithFormat:@"%@%@",WEICHAT_UID_PREFIX,user.uid],
                                    @"nickname": user.nickname,
                                    @"profileImage": user.icon,
                                    @"city": user.rawData[@"city"],
                                    @"country": user.rawData[@"country"],
                                    @"language": user.rawData[@"language"],
                                    @"sourceData_sex": user.rawData[@"sex"],
                                    @"sourceData_unionid": user.rawData[@"unionid"],
                                    //授权信息:
                                    @"token": user.credential.token,
                                    @"access_token": user.credential.rawData[@"access_token"],
                                    @"refresh_token": user.credential.rawData[@"refresh_token"],
                                    @"expired_date": [NSString stringFromDateWithShareFormat:user.credential.expired],
                                    @"secret": user.credential.secret?user.credential.secret:[NSNull null],  //通常为nil
                                    },
                            };
               } else if (state == SSDKResponseStateFail) {
                   dict = @{@"result": @(-2)};
               }else {
                   dict = @{@"result": @(-1)};
                   NSInteger errCode = [error code];
                   NSString *errDesc = [NSString stringWithFormat:@"%@",[error userInfo]];
                   NSLog(@"%ld, %@",(long)errCode, errDesc);
               }

               [CocosLua callLuaFunction:callback res:[NSString dictionaryToCJsonString:dict]];
           }];
}

+ (void)logout {
    [ShareSDK cancelAuthorize:SSDKPlatformTypeWechat];
}

+ (void)share:(NSDictionary *)params {
    NSMutableDictionary *shareParams = [NSMutableDictionary dictionaryWithDictionary:[ShareData handleShareData:params]];
    int callback = [ShareData handleShareCallback:params];
    int type = [params[@"share_type"] intValue];
    SSDKPlatformType platformSubType = (SSDKPlatformType)type;
    
    [ShareSDK share:platformSubType
         parameters:shareParams
     onStateChanged:^(SSDKResponseState state, NSDictionary *userData, SSDKContentEntity *contentEntity, NSError *error) {
         if (state == SSDKResponseStateSuccess) {
             NSLog(@"分享成功");
             
             NSDictionary *dict = @{@"result": @(0)};
             [CocosLua callLuaFunction:callback res:[NSString dictionaryToCJsonString:dict]];
             
         } else if (state == SSDKResponseStateFail) {
             NSLog(@"分享失败:%@----%ld", [error localizedDescription], (long)[error code]);
             
             NSDictionary *dict = @{@"result": @(-1)};
             [CocosLua callLuaFunction:callback res:[NSString dictionaryToCJsonString:dict]];
             
         } else if(state == SSDKResponseStateCancel) {
             NSLog(@"分享取消");
             
             NSDictionary *dict = @{@"result": @(-2)};
             [CocosLua callLuaFunction:callback res:[NSString dictionaryToCJsonString:dict]];
         }
         
     }];
}

//不接入SDK的微信朋友圈分享，见: http://blog.csdn.net/qq_30970529/article/details/50983092 或 http://www.jianshu.com/p/38cec090da39
//以及demo工程: https://github.com/100apps/openshare.git
//经测试，使用该方法，在牌乐门中分享到朋友圈，别人还是看不到
+ (void)share2:(NSDictionary*)params
{
	int share_type = [params[@"share_type"] intValue];
	NSString *title = params[@"share_title"];
	NSString *content = params[@"share_content"];
	NSString *image = params[@"share_img"];
	int imageFrom = [params[@"share_img_from"] intValue];// 图片来源(1:截图)
	NSString *urlStr = params[@"share_url"];
	int callback = [params[@"callback"] intValue];//回调
	
	NSURL *url = [NSURL URLWithString:urlStr];
	if(url == nil)
		url = [NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
	BOOL useThis = url == nil && share_type == 23;//分享图片到微信朋友圈的才用这个方法
	if(!useThis)
	{
		[PlatformWechat share:params];
		return;
	}
	
	NSString *imagePath = nil;
	if (imageFrom == 1) { //截图
		imagePath = image;
	} else { // 本地资源
		NSString *DEFAULT_PATH = @"res/app_res/application_res/";
		NSString *resourcePath = [NSString stringWithFormat:@"%@%@", DEFAULT_PATH, image];
		imagePath = [[NSBundle mainBundle] pathForResource:resourcePath ofType:nil];
	}

	NSMutableDictionary *dic = [[NSMutableDictionary alloc] init];
	dic[@"result"] = @"1";//固定写法
	dic[@"returnFromApp"] = @"0";//固定写法
	dic[@"scene"] = @"1";//0=分享到好友，1=分享到朋友圈
	dic[@"sdkver"] = @"1.7.7";
	dic[@"command"] = @"1010";//文字分享为1020，其余都是1010

	dic[@"title"] = title;
	dic[@"description"] = content;//这句没用
	UIImage *img = [UIImage imageWithContentsOfFile:imagePath];
	dic[@"fileData"] = [PlatformWechat dataWithImage:img];
	dic[@"thumbData"] = [PlatformWechat dataWithImage:img scale:CGSizeMake(100, 100)];
	dic[@"objectType"] = @"2";//2=图片分享，3=网络音频分享，4=网络视频分享，5=链接分享，6=文件分享，7=应用消息分享，8=表情分享，36=小程序分享

	NSData *output=[NSPropertyListSerialization dataWithPropertyList:@{WX_APP_ID:dic} format:NSPropertyListBinaryFormat_v1_0 options:0 error:nil];
	[[UIPasteboard generalPasteboard] setData:output forPasteboardType:@"content"];
	
	NSString *str = [NSString stringWithFormat:@"weixin://app/%@/sendreq/?", WX_APP_ID];
	NSURL *nsurl = [NSURL URLWithString:str];
	[[UIApplication sharedApplication] openURL:nsurl];
}

+ (NSData *)dataWithImage:(UIImage *)image {
	return UIImageJPEGRepresentation(image, 1);
}

+ (NSData *)dataWithImage:(UIImage *)image scale:(CGSize)size {
	UIGraphicsBeginImageContext(size);
	[image drawInRect:CGRectMake(0,0, size.width, size.height)];
	UIImage* scaledImage =UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();
	return UIImageJPEGRepresentation(scaledImage, 1);
}

+ (int)supportShare3:(NSDictionary*)params
{
	return 1;
}

+ (void)share3:(NSDictionary*)params
{
	NSString *UIActivityTypeWeixin = @"com.tencent.xin.sharetimeline";
	//参考: https://www.jianshu.com/p/0816a30cf255
	int share_type = [params[@"share_type"] intValue];
	NSString *title = params[@"share_title"];
	NSString *content = params[@"share_content"];
	NSString *image = params[@"share_img"];
	int imageFrom = [params[@"share_img_from"] intValue];// 图片来源(1:截图)
	NSString *urlStr = params[@"share_url"];
	int callback = [params[@"callback"] intValue];//回调
	
	NSString *imagePath = nil;
	if (imageFrom == 1) { //截图
		imagePath = image;
	} else { // 本地资源
		NSString *DEFAULT_PATH = @"res/app_res/application_res/";
		NSString *resourcePath = [NSString stringWithFormat:@"%@%@", DEFAULT_PATH, image];
		imagePath = [[NSBundle mainBundle] pathForResource:resourcePath ofType:nil];
	}
	UIImage *img = [UIImage imageWithContentsOfFile:imagePath];
	
	NSMutableArray *items = [[NSMutableArray alloc] init];
	if(urlStr && [urlStr length] > 0)//H5分享
	{
		if(title && [title length] > 0)
			[items addObject:title];
		if(img)
			[items addObject:img];
//		if(content && [content length] > 0)//加入这个会导致分享到微信时:获取数据异常
//			[items addObject:content];
		NSURL *url = [NSURL URLWithString:urlStr];
		if(url == nil)
			url = [NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
		[items addObject:url];
	}
	else//图文分享
	{
//		if(title && [title length] > 0)//加入这个会导致分享到微信时:获取数据异常
//			[items addObject:title];
		if(img)
			[items addObject:img];
//		if(content && [content length] > 0)//加入这个会导致分享到微信时:获取数据异常
//			[items addObject:content];
	}
	UIActivityViewController *activityController = [[UIActivityViewController alloc] initWithActivityItems:items applicationActivities:nil];
	//不出现在活动项目
	activityController.excludedActivityTypes = @[UIActivityTypePostToFacebook,UIActivityTypePostToTwitter, UIActivityTypePostToWeibo,UIActivityTypeMessage,UIActivityTypeMail,UIActivityTypePrint,UIActivityTypeCopyToPasteboard,UIActivityTypeAssignToContact,UIActivityTypeSaveToCameraRoll,UIActivityTypeAddToReadingList,UIActivityTypePostToFlickr,UIActivityTypePostToVimeo,UIActivityTypePostToTencentWeibo,UIActivityTypeAirDrop,UIActivityTypeOpenInIBooks,
												 @"com.apple.mobilenotes.SharingExtension",@"com.apple.mobileslideshow.StreamShareService",
												 @"com.tencent.mqq.ShareExtension",//QQ
												 @"com.apple.NanoTimeKitCompanion.CreateWatchFace",//创建表盘
												 @"com.apple.CloudDocsUI.AddToiCloudDrive",//存储到文件
												 @"com.alipay.iphoneclient.ExtensionSchemeShare",//支付宝
												 @"com.apple.reminders.RemindersEditorExtension",//提醒事项
												 ];
	activityController.completionWithItemsHandler = ^(UIActivityType  _Nullable activityType, BOOL completed, NSArray * _Nullable returnedItems, NSError * _Nullable activityError) {
		NSLog(@"activityType:%@,completed:%d,error:%@", activityType, completed, activityError);
		NSDictionary *dict;
		if(completed)
		{
			if(!activityError)//success
			{
				if(activityType == nil)
					dict = @{@"result": @(0)};
				else if([activityType compare:UIActivityTypeWeixin] == NSOrderedSame)
					dict = @{@"result": @(0)};
				else
					dict = @{@"result": @(-1)};
			}
			else//error
			{
				dict = @{@"result": @(-1)};
			}
		}
		else//canceled
		{
			dict = @{@"result": @(-2)};
		}
		[CocosLua callLuaFunction:callback res:[NSString dictionaryToCJsonString:dict]];
	};
	
	UIViewController *viewController = [[[[UIApplication sharedApplication] delegate] window] rootViewController];
	if([activityController respondsToSelector:@selector(popoverPresentationController)])
		activityController.popoverPresentationController.sourceView = [viewController view];//加入这句为了解决在ipad上崩溃, 参考:http://www.cocoachina.com/bbs/read.php?tid=233799
//	[viewController.navigationController presentViewController:activityController animated:YES completion:nil];//这句调不出系统分享选择框
	[viewController presentViewController:activityController animated:YES completion:nil];//这句在iphone7上可以
}

+ (int)isClientInstalled {
    return [ShareSDK isClientInstalled:SSDKPlatformTypeWechat]?1:0;
}

@end

#endif//#if defined(DM_PLATFORM_IOS)
