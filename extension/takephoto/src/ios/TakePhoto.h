#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

@interface TakePhoto : NSObject<UINavigationControllerDelegate, UIImagePickerControllerDelegate>
{
	UIViewController *m_viewController;
	UIImagePickerController *m_imagePickerController;
	int m_callbackId;
	int m_outputWidth, m_outputHeight;
	NSString *m_savePath;
}

+(void)takePicture:(NSDictionary*)dict;
+(UIImage *)reSizeImage:(UIImage *)image toSize:(CGSize)reSize;

-(void)doTakePicture:(NSDictionary*)dict;

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info;
-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;

@end
