#if defined(DM_PLATFORM_ANDROID)

#include "../xianliao.h"

#define XIANLIAO_CLASS	"com.xishanju.plm.xianliao.XianliaoUtil"

extern void ext_callLuaCallbackInAndroid(JNIEnv *env, jint callback, jstring value, bool unregister);

void xianliao_onAppInit()
{
	bool ok;
	ext_callJavaStaticMethod(XIANLIAO_CLASS, "init", XIANLIAO_APPID, &ok);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_xianliao_XianliaoUtil_nativeNotifyLogin(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, true);
}

int xianliao_login(lua_State *L)
{
	return ext_callNativeStaticMethod(XIANLIAO_CLASS, "login", L, -1);
}

int xianliao_logout(lua_State *L)
{
	return ext_callNativeStaticMethod(XIANLIAO_CLASS, "logout", L, -1);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_xianliao_XianliaoUtil_nativeNotifyShare(JNIEnv *env, jclass clz, jint callback, jstring value)
{
	ext_callLuaCallbackInAndroid(env, callback, value, true);
}

int xianliao_share(lua_State *L)
{
	return ext_callNativeStaticMethod(XIANLIAO_CLASS, "share", L, -1);
}

#endif