#if defined(DM_PLATFORM_ANDROID)

#include "../takephoto.h"
#include <extension/luajavaoc/src/android/luajavaoc_android.h>

static void takephoto_onActivityResult(void *env, void *activity, int32_t request_code, int32_t result_code, void* result)
{
//	dmLogInfo("gwjgwj,takephoto result,activity=0x%x, requestCode=%d,resultCode=%d,result=0x%x", activity, request_code, result_code, result);
	ext_callJavaStaticMethodV("com.xishanju.plm.takephoto.TakePhoto", "onActivityResult", "(IILandroid/content/Intent;)V", request_code, result_code, result);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_takephoto_TakePhoto_nativeNotifyGetResult(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, true);
}

void takephoto_onAppInit(const char *environment)
{
	dmExtension::RegisterAndroidOnActivityResultListener(takephoto_onActivityResult);
}

int takephoto_get_photo(lua_State *L)
{
	dmLogInfo("gwjgwj,getphoto,activity=0x%x", dmGraphics::GetNativeAndroidActivity());
	return ext_callNativeStaticMethod("com.xishanju.plm.takephoto.TakePhoto", "getPhoto", L, -1);
}

#endif