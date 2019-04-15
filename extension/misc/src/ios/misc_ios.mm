#if defined(DM_PLATFORM_IOS)

#include "../misc.h"
#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#include <pthread.h>
#import "AudioToolbox/AudioServices.h"

int misc_installApk(lua_State *L)
{
	return 0;
}

int misc_canOpenApp(lua_State *L)
{
	return ext_callNativeStaticMethod("MiscFunc", "canOpenApp", L, -1);
}

int misc_getBundleResourcePath(lua_State *L)
{
	if(lua_gettop(L) <= 0)
	{
		dmLogError("must specify a filename");
		return 0;
	}
	if(!lua_isstring(L, -1))
	{
		dmLogError("must specify a filename");
		return 0;
	}
	const char *name = lua_tostring(L, -1);
	NSString *path = [[NSBundle mainBundle] pathForResource:[NSString stringWithUTF8String:name] ofType:nil];
	if(path)
	{
		lua_pushstring(L, [path UTF8String]);
		return 1;
	}
	return 0;
}

int misc_androidCheckPermission(lua_State *L)
{
	if(!lua_isstring(L, -1))
	{
		dmLogError("invalid param 1 to androidCheckPermission, expecting string");
		lua_pushstring(L, "denied");
		return 1;
	}
	const char *permission = lua_tostring(L, -1);
	lua_pushstring(L, "granted");
	return 1;
}

int misc_androidRequestPermissions(lua_State *L)
{
	return 0;
}

int misc_androidGetTargetSdkVersion(lua_State *L)
{
	lua_pushinteger(L, 0);
	return 1;
}

int misc_androidGotoAppSetting(lua_State *L)
{
	lua_pushstring(L, "not support");
	return 1;
}

int misc_getBatteryInfo(lua_State *L)
{
	UIDevice *device = [UIDevice currentDevice];
	device.batteryMonitoringEnabled = YES;
	int level = device.batteryLevel * 100;
	int plugged = 0;
	UIDeviceBatteryState state = device.batteryState;
	if(state == UIDeviceBatteryStateCharging || state == UIDeviceBatteryStateFull)
		plugged = 1;
	
	lua_pushinteger(L, level);
	lua_pushinteger(L, plugged);
	return 2;
}

int misc_getCurrentThreadId(lua_State *L)
{
	pthread_t thread = pthread_self();
	lua_pushinteger(L, (long)thread);
	return 1;
}

void misc_doVibrate(float timeInSeconds)
{
	SystemSoundID id = kSystemSoundID_Vibrate;
	if(timeInSeconds == 10000) id = 1519;//短振 3D touch中的peek振动反馈
	else if(timeInSeconds == 20000) id = 1520;//短振 3D touch中的pop振动反馈
	else if(timeInSeconds == 30000) id = 1521;//连续三次短振动
	AudioServicesPlaySystemSound(id);
}

int misc_goHome(lua_State *L)
{
	return 0;
}

void misc_doPrint(const char *msg)
{
	dmLogInfo("%s", msg);
}

#endif
