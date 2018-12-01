#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include <functional>
#include "../wechat.h"

#define WECHAT_CLASS	"com.xishanju.plm.wechat.PlatformWechat"

//com.xishanju.plm.wechat.PlatformWechat
extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_wechat_PlatformWechat_notifyLua(JNIEnv *env, jclass clz, jstring value)
{
	jstring value2 = (jstring)env->NewGlobalRef(value);
	auto func = [=](){
		JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
		JNIEnv *env2;
		vm->AttachCurrentThread(&env2, NULL);
		const char *value_ = env2->GetStringUTFChars(value2, 0);
		wechat_notifyLoginResult(value_);
		env2->ReleaseStringUTFChars(value2, value_);
		env2->DeleteGlobalRef(value2);
		vm->DetachCurrentThread();
	};
	ext_performInUpdateThread(func);
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

int wechat_shareWithIntent(lua_State *L)
{
	std::string param = ext_jsonFromLuaTable(L, -1);
	bool ok;
	std::string ret = ext_callJavaStaticMethod("com.xishanju.plm.wechat.ShareUtil", "shareWithIntent", param.c_str(), &ok);
	lua_pushboolean(L, ok);
	lua_pushstring(L, ret.c_str());
	return 2;
}

#endif
