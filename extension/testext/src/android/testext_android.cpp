#if defined(DM_PLATFORM_ANDROID)

#include "../testext.h"

int test_callnative(lua_State *L)
{
	int numRet = ext_callNativeStaticMethod("abc.def.ghi.TestExt", "theFunc", L, -1);
	return numRet;
}

#endif
