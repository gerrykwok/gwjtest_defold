#if defined(DM_PLATFORM_ANDROID)

#include "../testext.h"

int test_callnative(lua_State *L)
{
	return ext_callNativeStaticMethod("abc.def.ghi.TestExt", "theFunc", L, -1);
}

int test_centerWindow(lua_State *L)
{
	return 0;
}

#endif
