#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include <functional>
#include "../wechat.h"
#include <extension/luajavaoc/src/android/luajavaoc_android.h>

#define WECHAT_CLASS	"com.xishanju.plm.wechat.PlatformWechat"

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_wechat_PlatformWechat_notifyLua(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, true);
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
	ext_callLuaCallbackInAndroid(env, callback, value, true);
}

int wechat_shareWithSDK(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.wechat.ShareUtil", "shareWithSDK", L, -1);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_wechat_WeixinPayUtil_nativeNotifyPay(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, true);
}

int wechat_makePurchase(lua_State *L)
{
	return ext_callNativeStaticMethod("com.xishanju.plm.wechat.WeixinPayUtil", "makePurchase", L, -1);
}

#endif
