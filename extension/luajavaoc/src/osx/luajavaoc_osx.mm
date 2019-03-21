#if defined(DM_PLATFORM_OSX)

#include "../luacallback.h"

std::string ext_callNativeStaticMethodBase(const char *clazz, const char *method, lua_State *L, int idxParam, bool *ok)
{
	*ok = false;
	return "";
}

int ext_gettimeofday(long *sec, long *usec)
{
	return 0;
}

#endif
