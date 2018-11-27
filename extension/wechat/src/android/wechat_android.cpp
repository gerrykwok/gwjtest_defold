#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include "../wechat.h"

#define WECHAT_CLASS	"com.xishanju.plm.wechat.PlatformWechat"

//com.xishanju.plm.wechat.PlatformWechat
extern "C"
JNIEXPORT void JNICALL
Java_com_xishanju_plm_wechat_PlatformWechat_notifyLua(JNIEnv *env, jclass clz, jstring value) {
	const char *value_ = env->GetStringUTFChars(value, 0);
	wechat_notifyLoginResult(value_);
	env->ReleaseStringUTFChars(value, value_);
}

void wechat_onAppInit()
{
	ext_call_java_static_void_method(WECHAT_CLASS, "init", "(Landroid/content/Context;)V", dmGraphics::GetNativeAndroidActivity());
}

void wechat_login()
{
	ext_call_java_static_void_method(WECHAT_CLASS, "login", "()V");
}

void wechat_logout()
{
	ext_call_java_static_void_method(WECHAT_CLASS, "logout", "()V");
}

#endif
