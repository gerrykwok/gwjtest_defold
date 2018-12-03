#if defined(DM_PLATFORM_IOS)

#import "TakePhoto.h"

@implementation TakePhoto

+(void)takePicture:(NSDictionary*)dict
{
	TakePhoto* takePhoto=[[TakePhoto alloc] init];
	[takePhoto doTakePicture:dict];
}

-(void)doTakePicture:(NSDictionary*)dict
{
	NSString *path = [dict objectForKey:@"path"];
	BOOL fromCamera = [[dict objectForKey:@"fromeCamera"] boolValue];
	int callback = [[dict objectForKey:@"callback"] intValue];
	m_callbackId = callback;

	UIWindow *window = dmGraphics::GetNativeiOSUIWindow();
	UIViewController *controller = window.rootViewController;

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
		BOOL success;
		NSFileManager *fileManager = [NSFileManager defaultManager];
		NSError **error = nil;

		UIImage *image = [info objectForKey:UIImagePickerControllerEditedImage];

		NSLog(@"image,Orientation=%ld,size=%f,%f", image.imageOrientation, image.size.width, image.size.height);

		CGSize reSize = CGSizeMake(output_width, output_height);
		UIImage * imageReSize = [self reSizeImage:image toSize:reSize];
		//        NSLog(@"savePath2:%@", savePath);
		NSString *imageFile = savePath;//[documentsDirectory stringByAppendingPathComponent:@"temp.jpg"];
		//        NSLog(@"imageFile:%@", imageFile);

		success = [fileManager fileExistsAtPath:imageFile];
		if(success) {
			NSLog(@"file %@ is Exits", imageFile);
			success = [fileManager removeItemAtPath:imageFile error:error];
		}
		// 将图片压缩后,写入到文件
		BOOL result = [UIImageJPEGRepresentation(imageReSize, 1.0f) writeToFile:imageFile atomically:YES];

		NSLog(@"writeToFile:%@  result:%d", imageFile, result);

		[self notifyResult:@"success"];
	}else{
		[self notifyResult:@"failed"];
	}

	[controller dismissViewControllerAnimated:YES completion:^{}];
	[m_imagePickerController release];
	m_imagePickerController = nil;
}

-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
	UIWindow *window = dmGraphics::GetNativeiOSUIWindow();
	UIViewController *controller = window.rootViewController;
	[controller dismissViewControllerAnimated:YES completion:^{}];

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

@end

#endif
