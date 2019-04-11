#if defined(DM_PLATFORM_IOS)

#include <string>
#include "../qiniu.h"
#import "UploadImageBridge.h"
//#import "Json/JsonUtil.h"
//#import "lua/CocosLua.h"
#import "AFNetworking/AFHTTPRequestOperationManager.h"

@interface UploadImageBridge (Private)

+(BOOL) checkAndNotifyError:(NSString *)key token:(NSString *)token;

@end

@implementation UploadImageBridge

#define UPLOAD_UNDEFINE_KEY         "?"
#define UPLOAD_QINIU_KEY            "key"
#define UPLOAD_QINIU_TOKEN          "token"
#define UPLOAD_QINIU_MIME_TYPE      "mimetype"
#define UPLOAD_QINIU_FILE           "path"

#define UPLOAD_QINIU_URL            "upload.qiniu.com"

+(void)uploadFile:(NSDictionary *)dict
{
	if(dict == nil)
		return;

	if([dict count] == 0)
		return;

	int callback = [[dict objectForKey:@"callback"] intValue];
	
	NSString *key = [dict objectForKey:@UPLOAD_QINIU_KEY];
	NSString *token = [dict objectForKey:@UPLOAD_QINIU_TOKEN];
	NSString *mimeType = [dict objectForKey:@UPLOAD_QINIU_MIME_TYPE];
	NSString *filePath = [dict objectForKey:@UPLOAD_QINIU_FILE];
	if(mimeType == nil || [mimeType isEqual:@""])
	{
		mimeType = @"application/octet-stream";
	}
	
	BOOL checkError = [UploadImageBridge checkAndNotifyError:key token:token];
	if(checkError)
	{
		NSString *res = [NSString stringWithFormat:@"{\"result\":%d,\"errMsg\":\"key or token invalid\"}", -1];
		[UploadImageBridge callLua:callback withResult:[res UTF8String]];
		return;
	}
	
	NSError *error = nil;

	NSData *data = [NSData dataWithContentsOfFile:filePath options:NSDataReadingMappedIfSafe error:&error];
	if(error)
	{
		// 读取文件失败
		NSLog(@"---------读取文件失败-------------");
		NSString *res = [NSString stringWithFormat:@"{\"result\":%d,\"errMsg\":\"read file failed\"}", -1];
		[UploadImageBridge callLua:callback withResult:[res UTF8String]];
		return;
	}
	
	AFHTTPRequestOperationManager *manager = [AFHTTPRequestOperationManager manager];
	manager.responseSerializer = [AFHTTPResponseSerializer serializer];

	NSMutableDictionary *parameters = [NSMutableDictionary dictionary];
	NSString *fileName = key;
	if (key)
	{
		parameters[@UPLOAD_QINIU_KEY] = key;
	}
	else
	{
		fileName = @UPLOAD_UNDEFINE_KEY;
	}
	parameters[@UPLOAD_QINIU_TOKEN] = token;
	parameters[@UPLOAD_QINIU_MIME_TYPE] = mimeType;
	
	
	[manager POST:[NSString stringWithFormat:@"http://%@", @UPLOAD_QINIU_URL] parameters:parameters constructingBodyWithBlock:^(id<AFMultipartFormData> formData){
		[formData appendPartWithFileData:data name:@"file" fileName:fileName mimeType:mimeType];
	} success:^(AFHTTPRequestOperation *operation, id responseObject){
		// 上传成功
		NSLog(@"---------上传图片成功-------------");
		NSString *res = [NSString stringWithFormat:@"{\"result\":%d}", 0];
		[UploadImageBridge callLua:callback withResult:[res UTF8String]];
		
	} failure:^(AFHTTPRequestOperation *operation, NSError *error){
		// 上传失败
		NSString *res;
		NSLog(@"---------上传图片失败-------------");
		NSDictionary *userInfo = [error userInfo];
		NSLog(@"userInfo:%@", userInfo);
//		for(NSString *key in userInfo)
//		{
//			NSLog(@"userInfo[%@] = %@", key, userInfo[key]);
//		}
		NSString *desc = userInfo[NSLocalizedDescriptionKey];
		if([desc containsString:@"(614)"])
		{
			res = [NSString stringWithFormat:@"{\"result\":%d,\"errMsg\":\"file exists\"}", 1];
		}
		else
		{
			res = [NSString stringWithFormat:@"{\"result\":%d,\"errMsg\":\"upload failed\"}", -1];
		}
		[UploadImageBridge callLua:callback withResult:[res UTF8String]];
	}];
}

+(BOOL) checkAndNotifyError:(NSString *)key token:(NSString *)token
{
	if (token == nil || [token isEqualToString:@""]) {
		return YES;
	}
	
	return NO;
}

+(void)callLua:(int)callback withResult:(const char *)res
{
	if(callback > 0)
	{
		ext_invokeLuaCallbackWithString(callback, res);
		ext_unregisterLuaCallback(callback);
	}
}

@end

#endif
