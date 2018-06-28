//
//  TakePhoto.h
//
//
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
//#import <UIKit/UIImagePickerController.h>
#import "sys/utsname.h"
//g #import "AppController.h" 

@interface TakePhoto : NSObject<UIImagePickerControllerDelegate,UINavigationControllerDelegate>{
//g    AppController *appController;
    UIViewController *m_viewController;
    int m_luaCallback;
    BOOL iOS4;
    UIImagePickerController *imagePickerController;
    UIImage *choosenPhoto;
    int photoTag;
    NSString *savePath;
    int m_outWidth, m_outHeight;
//    UIImagePickerController *curPicker;
}

+(void)takePicture:(NSDictionary*)dict;

-(void)setViewController:(UIViewController*)controller;

-(void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info;
-(void)imagePickerControllerDidCancel:(UIImagePickerController *)picker;

-(int)getDir:(int)userId;
-(void)snapImageSourceTypeCamera;
-(void)snapImageSourceTypePhotoLibrary;
-(UIImage *)reSizeImage:(UIImage *)image toSize:(CGSize)reSize;
-(UIImage* )rotateImage:(UIImage *)image;
-(void)saveUserBackImage:(NSString *)backImageName andImage:(UIImage *)userImage;
-(void)setPhotoTag:(int)tag;
-(void)setSavePath:(NSString *)path;
-(void)setOutputWidth:(int)width andHeight:(int)height;

//g @property (assign) AppController *appController;
@property (assign) UIViewController *m_viewController;
@property (assign) int m_luaCallback;
@property (nonatomic, retain) UIImage *choosenPhoto;

@end