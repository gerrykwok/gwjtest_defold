//
//  UploadImageBridge.h
//
//

#import <Foundation/Foundation.h>

@interface UploadImageBridge : NSObject
{
}

+(void)compileIn;

/**
 * 上传图片
 * 参数说明
 * key      七牛存储路径
 * token    根据七牛后台设置
 * mimeType 默认为 application/octet-stream
 * filepath 图片路径
 */
+(void)uploadImage:(NSDictionary *)dict;


@end