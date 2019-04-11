#if defined(DM_PLATFORM_ANDROID)

#include "../qiniu.h"
#include <extension/luajavaoc/src/android/luajavaoc_android.h>

int qiniu_uploadFile(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.qiniu.QiniuUtil", "uploadFile", L, -1);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_qiniu_QiniuUtil_nativeNotifyUploadResult(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, true);
}

#endif