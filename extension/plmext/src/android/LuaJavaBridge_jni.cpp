#if defined(DM_PLATFORM_ANDROID)

#include <stdlib.h>
#include <functional>
#include "LuaJavaBridge_jni.h"
#include "../plmext.h"

extern int LuaJavaBridge_callLuaFunctionById(int functionId, const char *value);
extern int LuaJavaBridge_callLuaGlobalFunction(const char *luaFunctionName, const char *value);
extern int LuaJavaBridge_retainLuaFunctionById(int luaFunctionId);
extern int LuaJavaBridge_releaseLuaFunctionById(int luaFunctionId);
extern void COCOSEXT_runOnGLThread(const std::function<void(void)> &callback);

JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(callLuaFunctionWithString)(JNIEnv *env, jclass cls, jint functionId, jstring value)
{
	if(env == NULL) return 0;
	const char *value_ = env->GetStringUTFChars(value, 0);
	int ret = LuaJavaBridge_callLuaFunctionById(functionId, value_);
	env->ReleaseStringUTFChars(value, value_);
	return ret;
}

JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(callLuaGlobalFunctionWithString)(JNIEnv *env, jclass cls, jstring luaFunctionName, jstring value)
{
	if(env == NULL) return 0;
	const char *luaFunctionName_ = env->GetStringUTFChars(luaFunctionName, 0);
	const char *value_ = env->GetStringUTFChars(value, 0);
	int ret = LuaJavaBridge_callLuaGlobalFunction(luaFunctionName_, value_);
	env->ReleaseStringUTFChars(luaFunctionName, luaFunctionName_);
	env->ReleaseStringUTFChars(value, value_);
	return ret;
}

JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(retainLuaFunction)(JNIEnv *env, jclass cls, jint luaFunctionId)
{
	if(env == NULL) return 0;
	return LuaJavaBridge_retainLuaFunctionById(luaFunctionId);
}

JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(releaseLuaFunction)(JNIEnv *env, jclass cls, jint luaFunctionId)
{
	if(env == NULL) return 0;
	return LuaJavaBridge_releaseLuaFunctionById(luaFunctionId);
}

JNIEXPORT void JNICALL DEF_JNI_FUNCTION(runOnGLThread)(JNIEnv *env, jclass cls, jobject runnable)
{
	if(env == NULL) return;
	jobject runnable2 = env->NewGlobalRef(runnable);
	COCOSEXT_runOnGLThread([=]() {
//		LOG_printf("gwjgwj,runnable=0x%x", runnable2);
		JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
		JNIEnv *env2;
		vm->AttachCurrentThread(&env2, NULL);
		jclass clz = env2->GetObjectClass(runnable2);
		jmethodID method = env2->GetMethodID(clz, "run", "()V");
		env2->CallVoidMethod(runnable2, method);
		env2->DeleteGlobalRef(runnable2);
		vm->DetachCurrentThread();
	});
}

#endif
