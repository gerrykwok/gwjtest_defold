#import <Foundation/Foundation.h>

@interface TakePhoto : NSObject<UIImagePickerControllerDelegate>
{
	UIImagePickerController *m_imagePickerController;
	int m_callbackId;
}

+(void)takePicture:(NSDictionary*)dict;

-(void)doTakePicture:(NSDictionary*)dict;

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info;
-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;

@end
