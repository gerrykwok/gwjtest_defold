#if defined(DM_PLATFORM_OSX)

#include "../umeng.h"

void umeng_onAppInit(const char *environment)
{
}

void umeng_onAppActivate()
{
}

void umeng_onAppDeactivate()
{
}

int umeng_sendEvent(lua_State *L)
{
	return 0;
}

#endif