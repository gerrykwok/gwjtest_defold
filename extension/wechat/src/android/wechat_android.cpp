#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include <functional>
#include "../wechat.h"

#define WECHAT_CLASS	"com.xishanju.plm.wechat.PlatformWechat"

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_wechat_PlatformWechat_notifyLua(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	if(callback <= 0) return;
	jstring value2 = (jstring)env->NewGlobalRef(value);
	auto func = [=](){
		JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
		JNIEnv *env2;
		vm->AttachCurrentThread(&env2, NULL);
		const char *value_ = env2->GetStringUTFChars(value2, 0);
		
		ext_invokeLuaCallbackWithString(callback, value_);
		ext_unregisterLuaCallback(callback);
		
		env2->ReleaseStringUTFChars(value2, value_);
		env2->DeleteGlobalRef(value2);
		vm->DetachCurrentThread();
	};
	ext_performInUpdateThread(func);
}

void wechat_onAppInit()
{
	bool ok;
	ext_callJavaStaticMethod(WECHAT_CLASS, "init", WECHAT_APPID, &ok);
}

int wechat_login(lua_State *L)
{
	return ext_callNativeStaticMethod(WECHAT_CLASS, "login", L, -1);
}

int wechat_logout(lua_State *L)
{
	return ext_callNativeStaticMethod(WECHAT_CLASS, "logout", L, -1);
}

int wechat_shareWithIntent(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.wechat.ShareUtil", "shareWithIntent", L, -1);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_wechat_ShareUtil_nativeNotifyShare(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	if(callback <= 0) return;
	jstring value2 = (jstring)env->NewGlobalRef(value);
	auto func = [=](){
		JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
		JNIEnv *env2;
		vm->AttachCurrentThread(&env2, NULL);
		const char *value_ = env2->GetStringUTFChars(value2, 0);

		ext_invokeLuaCallbackWithString(callback, value_);
		ext_unregisterLuaCallback(callback);
		
		env2->ReleaseStringUTFChars(value2, value_);
		env2->DeleteGlobalRef(value2);
		vm->DetachCurrentThread();
	};
	ext_performInUpdateThread(func);
}

int wechat_shareWithSDK(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.wechat.ShareUtil", "shareWithSDK", L, -1);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_wechat_WeixinPayUtil_nativeNotifyPay(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	if(callback <= 0) return;
	jstring value2 = (jstring)env->NewGlobalRef(value);
	auto func = [=](){
		JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
		JNIEnv *env2;
		vm->AttachCurrentThread(&env2, NULL);
		const char *value_ = env2->GetStringUTFChars(value2, 0);

		ext_invokeLuaCallbackWithString(callback, value_);
		ext_unregisterLuaCallback(callback);

		env2->ReleaseStringUTFChars(value2, value_);
		env2->DeleteGlobalRef(value2);
		vm->DetachCurrentThread();
	};
	ext_performInUpdateThread(func);
}

int wechat_makePurchase(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.wechat.WeixinPayUtil", "makePurchase", L, -1);
}

#endif
