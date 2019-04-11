#if defined(DM_PLATFORM_IOS)

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
        std::string json_str;
        [JsonUtil json_object_begin:json_str];
        [JsonUtil json_object_append_number_item:json_str key:"result" value:-1];
        [JsonUtil json_object_end:json_str];
        [CocosLua callLuaFunction:callback res:json_str.c_str()];
        
        return;
    }
    
    NSError *error = nil;

    NSData *data = [NSData dataWithContentsOfFile:filePath options:NSDataReadingMappedIfSafe error:&error];
    if(error)
    {
        // 读取文件失败
        NSLog(@"---------读取文件失败-------------");
        
//        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
//        [dictionary setObject:@RESULT_FAIL forKey:@JSON_KEY_RESULT];
//        string jsonRes = [Commom buildJsonString:dictionary];
//        
//        [Commom executeCallbackMethod:callback res:jsonRes.c_str()];
        
        std::string json_str;
        [JsonUtil json_object_begin:json_str];
        [JsonUtil json_object_append_number_item:json_str key:"result" value:-1];
        [JsonUtil json_object_end:json_str];
        [CocosLua callLuaFunction:callback res:json_str.c_str()];
        
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
        
//        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
//        [dictionary setObject:@RESULT_OK forKey:@JSON_KEY_RESULT];
//        string jsonRes = [Commom buildJsonString:dictionary];
//
//        [Commom executeCallbackMethod:callback res:jsonRes.c_str()];

        std::string json_str;
        [JsonUtil json_object_begin:json_str];
        [JsonUtil json_object_append_number_item:json_str key:"result" value:0];
        [JsonUtil json_object_end:json_str];
        [CocosLua callLuaFunction:callback res:json_str.c_str()];
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error){
        
        // 上传失败
        NSLog(@"---------上传图片失败-------------");
        NSLog(@"%@", [error userInfo]);
        
//        NSMutableDictionary *dictionary = [NSMutableDictionary dictionary];
//        [dictionary setObject:@RESULT_FAIL forKey:@JSON_KEY_RESULT];
//        string jsonRes = [Commom buildJsonString:dictionary];
//        [Commom executeCallbackMethod:callback res:jsonRes.c_str()];
        
        std::string json_str;
        [JsonUtil json_object_begin:json_str];
        [JsonUtil json_object_append_number_item:json_str key:"result" value:-1];
        [JsonUtil json_object_end:json_str];
        [CocosLua callLuaFunction:callback res:json_str.c_str()];
    }];
}

+(BOOL) checkAndNotifyError:(NSString *)key token:(NSString *)token
{
    if (token == nil || [token isEqualToString:@""]) {
        return YES;
    }
    
    return NO;
}

@end

#endif
