//
//  ShareData.h
//  ykctest
//
//  Created by Xiejiajian on 2017/8/2.
//
//

#import <Foundation/Foundation.h>

@interface ShareData : NSObject

+ (NSDictionary *)handleShareData:(NSDictionary *)params;

+ (int)handleShareCallback:(NSDictionary *)params;

@end
