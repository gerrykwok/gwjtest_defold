#if defined(DM_PLATFORM_WINDOWS)

#include "../luacallback.h"

std::string ext_callNativeStaticMethodBase(const char *clazz, const char *method, lua_State *L, int idxParam, bool *ok)
{
	*ok = false;
	return "";
}

#endif
