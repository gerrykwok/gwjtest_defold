#if defined(DM_PLATFORM_IOS)

#include "../takephoto.h"

void takephoto_onAppInit(const char *environment)
{
}

int takephoto_get_photo(lua_State *L)
{
	return ext_callNativeStaticMethod("TakePhoto", "takePicture", L, -1);
}

#endif