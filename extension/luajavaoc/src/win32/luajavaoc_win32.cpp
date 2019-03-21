#if defined(DM_PLATFORM_WINDOWS)

#include "../luacallback.h"

std::string ext_callNativeStaticMethodBase(const char *clazz, const char *method, lua_State *L, int idxParam, bool *ok)
{
	*ok = false;
	return "";
}

int ext_gettimeofday(long *sec, long *usec)
{
	LARGE_INTEGER liTime, liFreq;
	QueryPerformanceFrequency(&liFreq);
	QueryPerformanceCounter(&liTime);
	long a, b;
	a = (long)(liTime.QuadPart / liFreq.QuadPart);
	b = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - a * 1000000.0);
	if(sec) *sec = a;
	if(usec) *usec = b;
	return 0;
}

#endif
