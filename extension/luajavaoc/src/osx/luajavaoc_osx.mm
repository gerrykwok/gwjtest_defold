#if defined(DM_PLATFORM_OSX)

#include "../luacallback.h"
#include <sys/time.h>

std::string ext_callNativeStaticMethodBase(const char *clazz, const char *method, lua_State *L, int idxParam, bool *ok)
{
	*ok = false;
	return "";
}

int ext_gettimeofday(long *sec, long *usec)
{
	int ret;
	struct timeval now;
	ret = gettimeofday(&now, NULL);
	if(ret == 0)
	{
		if(sec) *sec = now.tv_sec;
		if(usec) *usec = now.tv_usec;
	}
	return ret;
}

#endif
