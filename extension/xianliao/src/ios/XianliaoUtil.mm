#if defined(DM_PLATFORM_IOS)

#import "../xianliao.h"
#import "XianliaoSDK_iOS/SugramApiManager.h"
#import "XianliaoSDK_iOS/SugramApiObject.h"
#import "XianliaoUtil.h"

@implementation XianliaoUtil

+(void)initXianliao:(NSString*)appid
{
	[SugramApiManager registerApp:appid];
}

+(void)login:(NSDictionary *)params
{
	int callback = [params[@"callback"] intValue];//回调
	[SugramApiManager loginState:nil fininshBlock:^(SugramLoginCallBackType callBackType, NSString *code, NSString *state) {
//		NSLog(@"callbacktype:%lu, code:%@, state:%@", callBackType, code, state);
		int errCode = 0;
		switch(callBackType)
		{
		case SugramLoginSuccessType: errCode = 0; break;
		case SugramLoginCancelType: errCode = -2; break;
		default: errCode = -1; break;
		}
		NSString *res = [NSString stringWithFormat:@"{\"errCode\":%d,\"code\":\"%@\"}", errCode, code];
		if(callback > 0)
		{
			ext_invokeLuaCallbackWithString(callback, [res UTF8String]);
			ext_unregisterLuaCallback(callback);
		}
	}];
}

+(void)share:(NSDictionary *)params
{
	NSString *type = params[@"type"];
	int callback = [params[@"callback"] intValue];
	NSString *title = params[@"title"];
	NSString *text = params[@"text"];
	NSString *url = params[@"url"];
	NSString *image = params[@"image"];
	int imageFrom = 1;
	id imgFrom = params[@"imageFrom"];
	if(imgFrom) imageFrom = [imgFrom intValue];
	NSString *imagePath = nil;
	if(image)
	{
		if(imageFrom == 2)
		{
			imagePath = [[NSBundle mainBundle] pathForResource:image ofType:nil];
		}
		else
		{
			imagePath = image;
		}
	}

	SugramShareBaseObject *baseObject = nil;
	if([type compare:@"text"] == NSOrderedSame)
	{
		SugramShareTextObject *object = [[SugramShareTextObject alloc] init];
		object.title = title;
		object.text = text;
		baseObject = object;
	}
	else if([type compare:@"image"] == NSOrderedSame)
	{
		SugramShareImageObject *object = [[SugramShareImageObject alloc] init];
		object.imageData = [NSData dataWithContentsOfFile:imagePath];
		baseObject = object;
		NSUInteger imageSize = [object.imageData length];
		if(imageSize > 512*1024)
		{
			NSLog(@"gwjgwj,image is bigger than 512KB, %ldKB", (unsigned long)imageSize);
			[XianliaoUtil notifyShare:callback withCode:-1];
			return;
		}
	}
	else if([type compare:@"app"] == NSOrderedSame)
	{
		SugramShareGameObject *gameObject = [[SugramShareGameObject alloc] init];
		gameObject.roomId = @" ";
		gameObject.roomToken = @"token";
		gameObject.title = title;
		gameObject.text = text;
		gameObject.imageData = [NSData dataWithContentsOfFile:imagePath];
		gameObject.androidDownloadUrl = url;
		gameObject.iOSDownloadUrl = url;
		baseObject = gameObject;
		NSUInteger imageSize = [gameObject.imageData length];
		if(imageSize > 32*1024)
		{
			NSLog(@"gwjgwj,image is bigger than 32KB, %ldKB", (unsigned long)imageSize);
			[XianliaoUtil notifyShare:callback withCode:-1];
			return;
		}
	}
	[SugramApiManager share:baseObject fininshBlock:^(SugramShareCallBackType callBackType) {
		int errCode = 0;
		switch(callBackType)
		{
		case SugramShareSuccesslType: errCode = 0; break;
		case SugramShareCancelType: errCode = -2; break;
		default: errCode = -1; break;
		}
		[XianliaoUtil notifyShare:callback withCode:errCode];
	}];
}

+(void)notifyShare:(int)callback withCode:(int)errCode
{
	NSString *res = [NSString stringWithFormat:@"{\"errCode\":%d}", errCode];
	[XianliaoUtil notifyShare:callback withResult:res];
}

+(void)notifyShare:(int)callback withResult:(NSString*)res
{
	if(callback <= 0) return;
	ext_invokeLuaCallbackWithString(callback, [res UTF8String]);
	ext_unregisterLuaCallback(callback);
}

@end

#endif
