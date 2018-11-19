#if defined(DM_PLATFORM_IOS)

//
//  ShareData.m
//  ykctest
//
//  Created by Xiejiajian on 2017/8/2.
//
//

#import "ShareData.h"
#import <ShareSDK/ShareSDK.h>
#import "NSString+ShareUtil.h"

@implementation ShareData

+ (NSDictionary *)handleShareData:(NSDictionary *)params {
    if (!params.allKeys.count) return nil;
    
    // 分享平台
    int type = [params[@"share_type"] intValue];
    // 回调
//    int callback = [params[@"callback"] intValue];
    // 分享内容
    NSString *content = params[@"share_content"];
    // 分享标题
    NSString *title = params[@"share_title"];
    // 分享链接
    NSString *urlStr = params[@"share_url"];
    // 分享图片
    NSString *image = params[@"share_img"];
    // 图片来源(1:截图)
    int imageFrom = [params[@"share_img_from"] intValue];
    // 标题点
    NSString *titlePointCode = params[@"title_point_code"];
    // jpeg压缩质量
    float jpgQuality = [params[@"jpeg_quality"] floatValue];
    
    if (title && ![titlePointCode isEqualToString:@""]) {
        NSString *emoji = [NSString getEmojiWithUnicode:titlePointCode];
        title = [NSString stringWithFormat:@"%@%@", emoji, title];
    }
    
    NSString *imagePath = nil;
    if (imageFrom == 1) { //截图
        imagePath = image;
    } else { // 本地资源
        NSString *DEFAULT_PATH = @"res/app_res/application_res/";
        NSString *resourcePath = [NSString stringWithFormat:@"%@%@", DEFAULT_PATH, image];
        imagePath = [[NSBundle mainBundle] pathForResource:resourcePath ofType:nil];
    }
    
    CGFloat jpegQuality = jpgQuality / 100.0f;
    SSDKImage *img = [[SSDKImage alloc] initWithImage:[UIImage imageWithContentsOfFile:imagePath]
                                               format:SSDKImageFormatJpeg
                                             settings:@{SSDKImageSettingQualityKey: @(jpegQuality)}];
    
    SSDKPlatformType platformSubType = (SSDKPlatformType)type;
    NSMutableDictionary *shareParams = [NSMutableDictionary dictionary];
    NSURL *url = [NSURL URLWithString:urlStr];
	if(url == nil)
		url = [NSURL URLWithString:[urlStr stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    switch (platformSubType) {
        case SSDKPlatformSubTypeQQFriend:
            [shareParams SSDKSetupQQParamsByText:content title:title url:url thumbImage:nil image:img type:SSDKContentTypeAuto forPlatformSubType:platformSubType];
            break;
        case SSDKPlatformSubTypeWechatTimeline:
        case SSDKPlatformSubTypeWechatSession:
            [shareParams SSDKSetupWeChatParamsByText:content title:title url:url thumbImage:nil image:img musicFileURL:nil extInfo:nil fileData:nil emoticonData:nil type:SSDKContentTypeAuto forPlatformSubType:platformSubType];
            break;
//        case SSDKPlatformTypeAliSocial:
//            [shareParams SSDKSetupAliSocialParamsByText:content image:img title:title url:url type:SSDKContentTypeAuto platformType:platformSubType];
//            break;
        default:
            break;
    }
    
    return shareParams;
}

+ (int)handleShareCallback:(NSDictionary *)params {
    // 回调
    return [params[@"callback"] intValue];
}


@end

#endif
