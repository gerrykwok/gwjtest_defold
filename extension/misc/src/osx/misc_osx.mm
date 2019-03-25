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

struct DataContext
{
	float m_elapsed;
	int m_xWnd, m_yWnd;
	float m_timeInSeconds;
	unsigned int m_handleUpdate;
};

void misc_doVibrate(float timeInSeconds)
{
	DataContext *data = new DataContext;
	data->m_elapsed = 0;
	NSWindow *mainWnd = dmGraphics::GetNativeOSXNSWindow();
	data->m_xWnd = mainWnd.frame.origin.x; data->m_yWnd = mainWnd.frame.origin.y;
	data->m_timeInSeconds = timeInSeconds;

	long sec, usec;
	ext_gettimeofday(&sec, &usec);
	data->m_handleUpdate = ext_scheduleUpdate([=]() {
		int offsetX, offsetY;
		bool bEnd;
		if(data->m_elapsed < data->m_timeInSeconds)
		{
			offsetX = rand() % 10 * (rand() > 16384 ? 1 : -1);
			offsetY = rand() % 10 * (rand() > 16384 ? 1 : -1);

			long sec2, usec2;
			ext_gettimeofday(&sec2, &usec2);
			data->m_elapsed = sec2 - sec + (float)(usec2 - usec) / 1000000.0f;
			bEnd = false;
		}
		else
		{
			offsetX = offsetY = 0;
			bEnd = true;
		}

		NSWindow *mainWnd = dmGraphics::GetNativeOSXNSWindow();
		[mainWnd setFrameOrigin:NSMakePoint(data->m_xWnd+offsetX, data->m_yWnd+offsetY)];

		if(bEnd)
		{
			ext_unscheduleUpdate(data->m_handleUpdate);
			delete data;
		}
	});
}

#endif
