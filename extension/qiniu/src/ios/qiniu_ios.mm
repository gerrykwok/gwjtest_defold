#if defined(DM_PLATFORM_IOS)

#include "../qiniu.h"

int qiniu_uploadFile(lua_State *L)
{
	return ext_callNativeStaticMethod("UploadImageBridge", "uploadFile", L, -1);
}

#endif