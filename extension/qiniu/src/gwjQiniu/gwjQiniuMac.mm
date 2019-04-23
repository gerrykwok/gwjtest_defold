#if defined(DM_PLATFORM_OSX)

#include <string>
#include <functional>
#import <Foundation/Foundation.h>
#import "../ios/AFNetworking/AFHTTPRequestOperationManager.h"

#define UPLOAD_UNDEFINE_KEY         "?"
#define UPLOAD_QINIU_KEY            "key"
#define UPLOAD_QINIU_TOKEN          "token"
#define UPLOAD_QINIU_MIME_TYPE      "mimetype"
#define UPLOAD_QINIU_FILE           "filepath"
#define UPLOAD_QINIU_FILE_SUFFIX    "file_suffix"

#define UPLOAD_QINIU_URL            "upload.qiniu.com"

BOOL checkAndNotifyError(NSString *key, NSString *token)
{
	if (token == nil || [token isEqualToString:@""]) {
		return YES;
	}
	return NO;
}

extern "C" void Qiniu_mac_upload(const char *sToken, const char *sName, const char *sLocalPath, const std::function<void(int, const char*)> &callback)
{
	NSString *key = [NSString stringWithUTF8String:sName];
	NSString *token = [NSString stringWithUTF8String:sToken];
	NSString *mimeType = nil;
	NSString *filePath = [NSString stringWithUTF8String:sLocalPath];
	if(mimeType == nil || [mimeType isEqual:@""])
	{
		mimeType = @"application/octet-stream";
	}
	
	BOOL checkError = checkAndNotifyError(key, token);
	if(checkError)
	{
		callback(-1, "key or token is null");
		return;
	}
	
	NSError *error = nil;
	
	NSData *data = [NSData dataWithContentsOfFile:filePath options:NSDataReadingMappedIfSafe error:&error];
	if(error)
	{
		// 读取文件失败
		NSLog(@"---------读取文件失败-------------");
		callback(-1, "read file failed");
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
	
	std::function<void(int,const char*)> *func = new std::function<void(int,const char*)>(callback);
	[manager POST:[NSString stringWithFormat:@"http://%@", @UPLOAD_QINIU_URL] parameters:parameters constructingBodyWithBlock:^(id<AFMultipartFormData> formData){
		[formData appendPartWithFileData:data name:@"file" fileName:fileName mimeType:mimeType];
	} success:^(AFHTTPRequestOperation *operation, id responseObject){
		
		// 上传成功
		NSLog(@"---------上传图片成功-------------");
		
		(*func)(0, "success");
		delete func;
	} failure:^(AFHTTPRequestOperation *operation, NSError *error){
		
		// 上传失败
		NSLog(@"---------上传图片失败-------------");
		NSLog(@"%@", [error userInfo]);
		
		(*func)(1, "upload failed");
		delete func;
	}];
}

#endif
