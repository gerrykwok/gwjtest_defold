#if defined(DM_PLATFORM_IOS)

#include <dmsdk/sdk.h>
#import "TakePhoto.h"
#import "../testext.h"

@implementation TakePhoto

+(void)takePicture:(NSDictionary*)dict
{
	TakePhoto* takePhoto=[[TakePhoto alloc] init];
	[takePhoto doTakePicture:dict];
}

-(id)init
{
	if((self = [super init]))
	{
		m_outputWidth = 320;
		m_outputHeight = 320;
	}

	return self;
}

-(void)doTakePicture:(NSDictionary*)dict
{
	NSString *path = [dict objectForKey:@"path"];
	BOOL fromCamera = [[dict objectForKey:@"fromeCamera"] boolValue];
	int callback = [[dict objectForKey:@"callback"] intValue];
	m_callbackId = callback;
	m_savePath = path;

	UIWindow *window = dmGraphics::GetNativeiOSUIWindow();
	UIViewController *controller = window.rootViewController;
	m_viewController = controller;

	@try
	{
		if (m_imagePickerController == nil) {
			m_imagePickerController = [[UIImagePickerController alloc]init];
			m_imagePickerController.sourceType = fromCamera ? UIImagePickerControllerSourceTypeCamera : UIImagePickerControllerSourceTypePhotoLibrary;
			m_imagePickerController.delegate = self;
			m_imagePickerController.allowsEditing = YES;
		}
		[controller presentViewController:m_imagePickerController animated:YES completion:^{}];
	}@catch(NSException *e)
	{
	}@finally
	{
	}
}

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
	NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
	if ([mediaType isEqualToString:@"public.image"]){
		UIImage *image = [info objectForKey:UIImagePickerControllerEditedImage];

		NSLog(@"image,Orientation=%ld,size=%f,%f", (long)image.imageOrientation, image.size.width, image.size.height);

		CGSize reSize = CGSizeMake(m_outputWidth, m_outputHeight);
		UIImage * imageReSize = [TakePhoto reSizeImage:image toSize:reSize];
		NSString *imageFile = m_savePath;//[documentsDirectory stringByAppendingPathComponent:@"temp.jpg"];
//		NSLog(@"imageFile:%@", imageFile);

		NSFileManager *fileManager = [NSFileManager defaultManager];
		if([fileManager fileExistsAtPath:imageFile])
		{
			NSError *error;
			NSLog(@"file %@ is Exits", imageFile);
			[fileManager removeItemAtPath:imageFile error:&error];
		}
		// 将图片压缩后,写入到文件
		BOOL result = [UIImageJPEGRepresentation(imageReSize, 1.0f) writeToFile:imageFile atomically:YES];

		NSLog(@"writeToFile:%@  result:%d", imageFile, result);

		[self notifyResult:@"success"];
	}else{
		[self notifyResult:@"failed"];
	}

	[m_viewController dismissViewControllerAnimated:YES completion:^{}];
	[m_imagePickerController release];
	m_imagePickerController = nil;
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
	[m_viewController dismissViewControllerAnimated:YES completion:^{}];

	[m_imagePickerController release];
	m_imagePickerController = nil;
	[self notifyResult:@"cancel"];
}

-(void)notifyResult:(NSString*)res
{
	if(m_callbackId > 0)
	{
		ext_invokeLuaCallbackWithString(m_callbackId, [res UTF8String]);
		m_callbackId = 0;
	}
}

+ (UIImage *)reSizeImage:(UIImage *)image toSize:(CGSize)reSize
{
	UIGraphicsBeginImageContext(CGSizeMake(reSize.width, reSize.height));
	[image drawInRect:CGRectMake(0, 0, reSize.width, reSize.height)];
	UIImage *reSizeImage = UIGraphicsGetImageFromCurrentImageContext();
	UIGraphicsEndImageContext();

	return reSizeImage;
}

@end

#endif
