#if defined(DM_PLATFORM_OSX)

#import <Foundation/Foundation.h>
#include <pthread.h>
#include "../misc.h"

int misc_installApk(lua_State *L)
{
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
	lua_pushinteger(L, 100);
	lua_pushinteger(L, 1);
	return 2;
}

int misc_getCurrentThreadId(lua_State *L)
{
	pthread_t thread = pthread_self();
	lua_pushinteger(L, (long)thread);
	return 1;
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

#endif
