#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include <functional>
#include "../wechat.h"

#define WECHAT_CLASS	"com.xishanju.plm.wechat.PlatformWechat"

static std::function<void(void)> g_func;
static bool g_funcValid = false;

//com.xishanju.plm.wechat.PlatformWechat
extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_wechat_PlatformWechat_notifyLua(JNIEnv *env, jclass clz, jstring value)
{
	jstring value2 = (jstring)env->NewGlobalRef(value);
	g_func = [=](){
		JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
		JNIEnv *env2;
		vm->AttachCurrentThread(&env2, NULL);
		const char *value_ = env2->GetStringUTFChars(value2, 0);
		dmLogInfo("gwjgwj,value_=%s", value_);
		wechat_notifyLoginResult(value_);
		env2->ReleaseStringUTFChars(value2, value_);
		env2->DeleteGlobalRef(value2);
		vm->DetachCurrentThread();
	};
	g_funcValid = true;
	/*
	const char *value_ = env->GetStringUTFChars(value, 0);
	wechat_notifyLoginResult(value_);
	env->ReleaseStringUTFChars(value, value_);
	*/
}

void wechat_onAppInit()
{
	ext_call_java_static_void_method(WECHAT_CLASS, "init", "(Landroid/content/Context;)V", dmGraphics::GetNativeAndroidActivity());
}

void wechat_login()
{
//	dmLogInfo("gwjgwj,lua thread:%d", pthread_self());
	ext_call_java_static_void_method(WECHAT_CLASS, "login", "()V");
}

void wechat_logout()
{
	ext_call_java_static_void_method(WECHAT_CLASS, "logout", "()V");
}

void wechat_onUpdate()
{
	if(g_funcValid)
	{
//		dmLogInfo("gwjgwj,update thread:%d", pthread_self());
		g_func();
		g_funcValid = false;
	}
}

#endif
