#if defined(DM_PLATFORM_IOS)

#import <UIKit/UIKit.h>

#import "TakePhoto.h"
#import "../../ios/CCLuaBridge.h"

@implementation TakePhoto

//g @synthesize appController;
@synthesize m_viewController;
@synthesize m_luaCallback;
@synthesize choosenPhoto;

+(void)compileIn
{
}

+(void)takePicture:(NSDictionary*)dict
{
    int fromCamera = [[dict objectForKey:@"fromCamera"] intValue];
    NSString *path = [dict objectForKey:@"path"];
    int width = [[dict objectForKey:@"width"] intValue];
    int height = [[dict objectForKey:@"height"] intValue];
    int luaCallback = [[dict objectForKey:@"callback"] intValue];

    UIWindow *window = dmGraphics::GetNativeiOSUIWindow();
    UIViewController *controller = window.rootViewController;

    TakePhoto* takePhoto=[[TakePhoto alloc] init];
    [takePhoto setViewController:controller];
    [takePhoto setSavePath:path];
    takePhoto.m_luaCallback = luaCallback;
    [takePhoto setOutputWidth:width andHeight:height];

    if (fromCamera) {
        [takePhoto snapImageSourceTypeCamera];
    }else{
        [takePhoto snapImageSourceTypePhotoLibrary];
    }
}

-(id)init {
    if((self = [super init])) {
        NSString *deviceVersion = [UIDevice currentDevice].systemVersion;
        int deviceVersionInt = [deviceVersion intValue];
        
        if(deviceVersionInt < 5)
            iOS4 = YES;
        else
            iOS4 = NO;
        
//g        appController = (AppController *)[[UIApplication sharedApplication] delegate];
        savePath=nil;
        m_luaCallback = -1;
        m_outWidth = 32;
        m_outHeight = 32;
    }
    
    return self;
}

-(void)setViewController:(UIViewController*)controller
{
    m_viewController = controller;
}

//打开摄像头
-(void)snapImageSourceTypeCamera{
    @try
    {
//g        appController.supportAllButUpsideDown = true;
        
//g        cocos2d::Director::getInstance()->stopAnimation();
        if (imagePickerController == nil) {
            imagePickerController = [[UIImagePickerController alloc]init];
            imagePickerController.sourceType = UIImagePickerControllerSourceTypeCamera;;
            imagePickerController.delegate = self;
            imagePickerController.allowsEditing =YES;
        }

//g        [appController.viewController presentViewController:imagePickerController animated:YES completion:nil];
        [m_viewController presentViewController:imagePickerController animated:YES completion:nil];
    }@catch(NSException *e)
    {
//g        appController.supportAllButUpsideDown = false;
    }@finally
    {
    }
}

//打开图片库
-(void)snapImageSourceTypePhotoLibrary{
    @try
    {
//g        appController.supportAllButUpsideDown = true;
        
//g        cocos2d::Director::getInstance()->stopAnimation();
        if (imagePickerController == nil) {
            imagePickerController = [[UIImagePickerController alloc]init];
            imagePickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;;
            imagePickerController.delegate = self;
            imagePickerController.allowsEditing =YES;
        }
//g        [appController.viewController presentViewController:imagePickerController animated:YES completion:^{}];
        [m_viewController presentViewController:imagePickerController animated:YES completion:^{}];
    }@catch(NSException *e)
    {
//g        appController.supportAllButUpsideDown = false;

    }@finally
    {
    }
}

//获取媒体
-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSString *mediaType = [info objectForKey:UIImagePickerControllerMediaType];
    if ([mediaType isEqualToString:@"public.image"]){
        BOOL success;
        NSFileManager *fileManager = [NSFileManager defaultManager];
        NSError **error = nil;
        
        UIImage *image = [info objectForKey:@"UIImagePickerControllerEditedImage"];

        NSLog(@"image,Orientation=%ld,wh=%f,%f", image.imageOrientation, image.size.width, image.size.height);
        
        CGSize reSize = CGSizeMake(m_outWidth, m_outHeight);
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
        
        //CFShow([[NSFileManager defaultManager] directoryContentsAtPath:[NSHomeDirectory() stringByAppendingString:@"/Documents"]]);
        [self notifyGetPhotoResult:@"1"];
    }else{
        [self notifyGetPhotoResult:@"0"];
    }
    
//g    appController.supportAllButUpsideDown = false;
    
//g    [appController.viewController dismissViewControllerAnimated:YES completion:^{}];
    [m_viewController dismissViewControllerAnimated:YES completion:^{}];
    [imagePickerController release];
    imagePickerController = nil;
    if (savePath!=nil) {
        [savePath release];
        savePath=nil;
    }
    self = nil;
//g    cocos2d::Director::getInstance()->startAnimation();
}

//取消获取媒体
-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
//    NSLog(@"imagePickerControllerDidCancel");
    
//g    appController.supportAllButUpsideDown = false;
    
//g    [appController.viewController dismissViewControllerAnimated:YES completion:^{}];
    [m_viewController dismissViewControllerAnimated:YES completion:^{}];
    
    [imagePickerController release];
    imagePickerController = nil;
    if (savePath!=nil) {
        [savePath release];
        savePath=nil;
    }
    self = nil;
//    CmgeUtils::getInstance()->finishGetUserPhoto(0);
//g    cocos2d::Director::getInstance()->startAnimation();
}

-(void)notifyGetPhotoResult:(NSString*)res
{
//    if(m_luaCallback >= 0)
//    {
//        const char *res_ = [res UTF8String];
//        LuaStack::getInstance()->pushString(res_);
//        LuaStack::getInstance()->executeFunctionByHandler(m_luaCallback, 1);
//
//        LuaStack::getInstance()->removeScriptHandler(m_luaCallback);
//        m_luaCallback = -1;
//    }

    if (m_luaCallback >= 0)
    {
        LuaBridge::pushLuaFunctionById(m_luaCallback);
        LuaBridge::getStack()->pushString([res UTF8String]);
        LuaBridge::getStack()->executeFunction(1);
        //LuaBridge::getStack()->removeScriptHandler(m_luaCallback);
        m_luaCallback = -1;
    }
}

-(void)dealloc {
    [super dealloc];
}

-(void)setPhotoTag:(int)tag
{
    photoTag = tag;
}

-(void)setSavePath:(NSString *)path
{
//    savePath=path;
    if (savePath!=nil) {
        [savePath release];
        savePath=nil;
    }
    savePath  = [[NSString alloc] initWithFormat:@"%@",path];
    NSLog(@"%@", savePath);
}

-(void)setOutputWidth:(int)width andHeight:(int)height
{
    m_outWidth = width;
    m_outHeight = height;
}

- (UIImage *)reSizeImage:(UIImage *)image toSize:(CGSize)reSize
{
    UIGraphicsBeginImageContext(CGSizeMake(reSize.width, reSize.height));
    [image drawInRect:CGRectMake(0, 0, reSize.width, reSize.height)];
    UIImage *reSizeImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    return reSizeImage;
}

-(void)saveUserBackImage:(NSString *)backImageName andImage:(UIImage *)userImage
{
//    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
//    NSString *documentsDirectory = [paths objectAtIndex:0];
//    NSString *imagesDirectory = [[NSString alloc] initWithFormat:@"%@",[documentsDirectory stringByAppendingPathComponent:@"user"]];
//    
//    NSString *fileName = [imagesDirectory stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.jpg",backImageName]];
//
//    NSFileManager *fileManager = [NSFileManager defaultManager];
//    if (![fileManager fileExistsAtPath:imagesDirectory]) {
//        [fileManager createDirectoryAtPath:imagesDirectory withIntermediateDirectories:YES attributes:nil error:nil];
//    }
//
//    NSData *imageData = [[NSData alloc] initWithData:UIImageJPEGRepresentation(userImage, 0.5)];
//    
//    NSString *Url = [[NSString alloc]initWithFormat:@"%savatar/%d/%d.jpg",ZjhConfig::getInstance()->upPicUrl.c_str(),[self getDir:photoTag],photoTag];
//    
//    NSURL *serverUrl = [NSURL URLWithString:Url];
//    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:serverUrl
//                                                           cachePolicy:NSURLRequestReloadIgnoringLocalCacheData
//                                                       timeoutInterval:10];
//    
//    long long now = (long long)([[NSDate date] timeIntervalSince1970] * 1000.0f);
//    NSString *lastModifiedStr  = [[NSString alloc] initWithFormat:@"%lld",now];
//    NSString *dataLen  = [[NSString alloc] initWithFormat:@"%d",imageData.length];
//    
//    [request setHTTPMethod:@"POST"];
//    [request setHTTPBody:imageData];
//    [request setValue:@"Zjh/2.0" forHTTPHeaderField:@"User-Agent"];
//    [request setValue:lastModifiedStr forHTTPHeaderField:@"x-lm"];
//    [request setValue:dataLen forHTTPHeaderField:@"x-fl"];
//    [request setValue:@"application/octet-stream" forHTTPHeaderField:@"Content-Type"];
//    
//    [NSURLConnection sendSynchronousRequest:request returningResponse:nil error:nil];    
//    [imageData writeToFile:fileName atomically:YES];
//    
//    [Url release];
//    [lastModifiedStr release];
//    [dataLen release];
//    [imageData release];
//    [imagesDirectory release];
    
    
}

-(int) getDir:(int) userId
{
	int dir = 0;
	for (int i = 0; i < 200000; i++) {
		long edge = (i + 1) * 100000;
		if (userId < edge) {
			dir = i;
			break;
		}
	}
	return dir;
}


- (UIImage* )rotateImage:(UIImage *)image {
    int kMaxResolution = 200;
    
    CGImageRef imgRef = image.CGImage;
    CGFloat width = CGImageGetWidth(imgRef);
    CGFloat height = CGImageGetHeight(imgRef);
    CGAffineTransform transform = CGAffineTransformIdentity;
    CGRect bounds = CGRectMake(0, 0, width, height);
    if (width > kMaxResolution || height > kMaxResolution) {
        CGFloat ratio = width  /  height;
        if (ratio >= 1 ) {
            bounds.size.width = kMaxResolution;
            bounds.size.height = bounds.size.width / ratio;
        }else {
            bounds.size.height = kMaxResolution;
            bounds.size.width = bounds.size.height * ratio;
        }
    }
    CGFloat scaleRatio = bounds.size.width / width;
    CGSize imageSize = CGSizeMake(CGImageGetWidth(imgRef), CGImageGetHeight(imgRef));
    CGFloat boundHeight;
    UIImageOrientation orient = image.imageOrientation;
    switch (orient) {
        case UIImageOrientationUp:
            //EXIF = 1
            transform = CGAffineTransformIdentity;
            break;
        case UIImageOrientationUpMirrored:
            //EXIF = 2
            transform = CGAffineTransformMakeTranslation(imageSize.width, 0.0);
            transform = CGAffineTransformScale(transform, -1.0, 1.0 );
            break;
        case UIImageOrientationDown:
            //EXIF = 3
            transform = CGAffineTransformMakeTranslation(imageSize.width, imageSize.height);
            transform = CGAffineTransformRotate(transform, M_PI);
            break;
        case UIImageOrientationDownMirrored:
            //EXIF = 4
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.height);
            transform = CGAffineTransformScale(transform, 1.0, -1.0);
            break;
        case UIImageOrientationLeftMirrored:
            //EXIF = 5
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, imageSize.width );
            transform = CGAffineTransformScale(transform, -1.0, 1.0);
            transform = CGAffineTransformRotate(transform, 3.0 * M_PI / 2.0 );
            break;
        case UIImageOrientationLeft:
            //EXIF = 6
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(0.0, imageSize.width);
            transform = CGAffineTransformRotate( transform, 3.0 * M_PI / 2.0   );
            break;
        case UIImageOrientationRightMirrored:
            //EXIF = 7
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeScale(-1.0, 1.0);
            transform = CGAffineTransformRotate( transform, M_PI / 2.0);
            break;
        case UIImageOrientationRight:
            //EXIF = 8
            boundHeight = bounds.size.height;
            bounds.size.height = bounds.size.width;
            bounds.size.width = boundHeight;
            transform = CGAffineTransformMakeTranslation(imageSize.height, 0.0);
            transform = CGAffineTransformRotate(transform, M_PI / 2.0 );
            break;
        default:
            [NSException raise:NSInternalInconsistencyException format:@"Invalid image orientation"];
    }
    UIGraphicsBeginImageContext(bounds.size);
    CGContextRef context = UIGraphicsGetCurrentContext();
    if (orient == UIImageOrientationRight || orient == UIImageOrientationLeft) {
        CGContextScaleCTM(context, -scaleRatio, scaleRatio);
        CGContextTranslateCTM(context, -height, 0);
    }
    else {
        CGContextScaleCTM(context, scaleRatio, -scaleRatio);
        CGContextTranslateCTM(context, 0, -height);
    }
    CGContextConcatCTM(context, transform );
    CGContextDrawImage(UIGraphicsGetCurrentContext(), CGRectMake(0, 0, width, height), imgRef);
    UIImage *imageCopy = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return imageCopy;
}

@end

#endif
