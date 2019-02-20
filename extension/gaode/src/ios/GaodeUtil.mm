#if defined(DM_PLATFORM_IOS)

#import "../gaode.h"
#import "GaodeUtil.h"
#import <AMapFoundationKit/AMapFoundationKit.h>
#import <AMapLocationKit/AMapLocationKit.h>

#define IOS8 ([[[UIDevice currentDevice] systemVersion] doubleValue] >=8.0 ? YES:NO)

@interface MyAmapLocationDelegage : NSObject<AMapLocationManagerDelegate>
@end

@implementation GaodeUtil

static AMapLocationManager *g_amapMgr;
static int g_luaCallback = 0;

+(void)locationInit:(NSDictionary*)dict
{
	NSString *apiKey = @GAODE_APIKEY_IOS_FORMAL;
	[AMapServices sharedServices].apiKey = apiKey;

	if(g_amapMgr == nil)
	{
		g_amapMgr = [[AMapLocationManager alloc] init];
		g_amapMgr.delegate = [[MyAmapLocationDelegage alloc] init];
		g_amapMgr.distanceFilter = 1;
		g_amapMgr.locatingWithReGeocode = YES;
	}
}

+(void)locationStart:(NSDictionary*)dict
{
	g_luaCallback = [[dict objectForKey:@"callback"] intValue];
	if(g_amapMgr)
	{
		[g_amapMgr setLocatingWithReGeocode:YES];
		[g_amapMgr startUpdatingLocation];
	}
}

+(void)locationStop:(NSDictionary*)dict
{
	ext_unregisterLuaCallback(g_luaCallback);
	g_luaCallback = 0;
	if(g_amapMgr)
	{
		[g_amapMgr stopUpdatingLocation];
	}
}

+(int)getLocationAuthorizationStatus:(NSDictionary*)dict//0=没有，1=还没询问状态，2=已经获得授权
{
	CLAuthorizationStatus status = [CLLocationManager authorizationStatus];
	switch(status)
	{
	// User has not yet made a choice with regards to this application
	case kCLAuthorizationStatusNotDetermined: return 1;

	// This application is not authorized to use location services.  Due
	// to active restrictions on location services, the user cannot change
	// this status, and may not have personally denied authorization
	case kCLAuthorizationStatusRestricted: return 0;

	// User has explicitly denied authorization for this application, or
	// location services are disabled in Settings.
	case kCLAuthorizationStatusDenied: return 0;

	// User has granted authorization to use their location at any time,
	// including monitoring for regions, visits, or significant location changes.
	//
	// This value should be used on iOS, tvOS and watchOS.  It is available on
	// MacOS, but kCLAuthorizationStatusAuthorized is synonymous and preferred.
	case kCLAuthorizationStatusAuthorizedAlways: return 2;

	// User has granted authorization to use their location only when your app
	// is visible to them (it will be made visible to them if you continue to
	// receive location updates while in the background).  Authorization to use
	// launch APIs has not been granted.
	//
	// This value is not available on MacOS.  It should be used on iOS, tvOS and
	// watchOS.
	case kCLAuthorizationStatusAuthorizedWhenInUse: return 2;

	// User has authorized this application to use location services.
	//
	// This value is deprecated or prohibited on iOS, tvOS and watchOS.
	// It should be used on MacOS.
	//	case kCLAuthorizationStatusAuthorized: return 2;
	default: return 1;
	}
}

+(void)jumpLocationSetting:(NSDictionary*)dict
{
	if(IOS8)
	{
		[[UIApplication sharedApplication] openURL:[NSURL URLWithString:UIApplicationOpenSettingsURLString]];
	}
}

@end

NSString* convertToJsonData(NSDictionary *dict)
{
	NSError *error;
	NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&error];
	NSString *jsonString;

	if (!jsonData)
	{
		NSLog(@"%@",error);
		return nil;
	}
	else
	{
		jsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
	}

	NSMutableString *mutStr = [NSMutableString stringWithString:jsonString];
	//去掉字符串中的空格
//	NSRange range = {0,jsonString.length};
//	[mutStr replaceOccurrencesOfString:@" " withString:@"" options:NSLiteralSearch range:range];
	//去掉字符串中的换行符
	NSRange range2 = {0,mutStr.length};
	[mutStr replaceOccurrencesOfString:@"\n" withString:@"" options:NSLiteralSearch range:range2];
	return mutStr;
}

@implementation MyAmapLocationDelegage

- (void)amapLocationManager:(AMapLocationManager *)manager didUpdateLocation:(CLLocation *)location reGeocode:(AMapLocationReGeocode *)reGeocode
{
	NSLog(@"update location:%@,geo:%@", location, reGeocode);
	if(reGeocode == nil)
	{
		CLGeocoder * geoCoder = [[CLGeocoder alloc] init];
		[geoCoder reverseGeocodeLocation:location completionHandler:^(NSArray *placemarks, NSError *error)
		{
			for (CLPlacemark * placemark in placemarks)
			{
				NSDictionary *test = [placemark addressDictionary];

//				NSLog(@"all values:");
//				NSArray *keys = [test allKeys];
//				for (NSString *key : keys)
//				{
//					NSLog(@"key:%@,value:%@", key, [test objectForKey:key]);
//				}

				NSString *city = [test objectForKey:@"City"];
				NSString *province = [test objectForKey:@"State"];
				NSLog(@"%@",[NSString stringWithFormat:@"经度:%3.5f,纬度:%3.5f,省份:%@,城市:%@", location.coordinate.longitude, location.coordinate.latitude, province, city]);
				if(g_luaCallback > 0)
				{
					NSMutableDictionary *dict = [NSMutableDictionary dictionary];
					[dict setObject:@"success" forKey:@"result"];
					[dict setObject:[NSNumber numberWithDouble:location.coordinate.longitude] forKey:@"longitude"];
					[dict setObject:[NSNumber numberWithDouble:location.coordinate.latitude] forKey:@"latitude"];
					[dict setObject:province forKey:@"province"];
					[dict setObject:city forKey:@"city"];
					[MyAmapLocationDelegage notifyLocationResult:dict];

					break;
				}
			}
		}];
	}
	else
	{
		NSString *city = reGeocode.city;
		NSString *province = reGeocode.province;
		CLLocationDegrees longitude = location.coordinate.longitude;
		CLLocationDegrees latitude = location.coordinate.latitude;
		if(g_luaCallback > 0)
		{
			NSMutableDictionary *dict = [NSMutableDictionary dictionary];
			[dict setObject:@"success" forKey:@"result"];
			[dict setObject:[NSNumber numberWithDouble:location.coordinate.longitude] forKey:@"longitude"];
			[dict setObject:[NSNumber numberWithDouble:location.coordinate.latitude] forKey:@"latitude"];
			[dict setObject:province forKey:@"province"];
			[dict setObject:city forKey:@"city"];
			[MyAmapLocationDelegage notifyLocationResult:dict];
		}
	}
}

+(void)notifyLocationResult:(NSDictionary*)dict
{
	if(g_luaCallback <= 0) return;
	NSString *res = convertToJsonData(dict);
	ext_invokeLuaCallbackWithString(g_luaCallback, [res UTF8String]);
}

@end

#endif
