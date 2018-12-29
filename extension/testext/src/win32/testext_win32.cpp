#if defined(DM_PLATFORM_WINDOWS)

#include "../testext.h"

int test_callnative(lua_State *L)
{
	return 0;
}

int test_printMacro(lua_State *L)
{
	dmLogInfo("MY_DEFINE=%d", MY_DEFINE);
	return 0;
}

#endif
