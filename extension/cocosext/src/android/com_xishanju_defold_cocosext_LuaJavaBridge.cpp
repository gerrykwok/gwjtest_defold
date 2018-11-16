#if defined(DM_PLATFORM_ANDROID)

#include "com_xishanju_defold_cocosext_LuaJavaBridge.h"

#include "CCLuaJavaBridge.h"

JNIEXPORT jint JNICALL Java_com_xishanju_defold_cocosext_LuaJavaBridge_callLuaFunctionWithString
	(JNIEnv *env, jclass cls, jint functionId, jstring value)
{
	if(env == NULL) return 0;
	const char *value_ = env->GetStringUTFChars(value, 0);
	int ret = LuaJavaBridge::callLuaFunctionById(functionId, value_);
	env->ReleaseStringUTFChars(value, value_);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_xishanju_defold_cocosext_LuaJavaBridge_callLuaGlobalFunctionWithString
	(JNIEnv *env, jclass cls, jstring luaFunctionName, jstring value)
{
	if(env == NULL) return 0;
	const char *luaFunctionName_ = env->GetStringUTFChars(luaFunctionName, 0);
	const char *value_ = env->GetStringUTFChars(value, 0);
	int ret = LuaJavaBridge::callLuaGlobalFunction(luaFunctionName_, value_);
	env->ReleaseStringUTFChars(luaFunctionName, luaFunctionName_);
	env->ReleaseStringUTFChars(value, value_);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_xishanju_defold_cocosext_LuaJavaBridge_retainLuaFunction
	(JNIEnv *env, jclass cls, jint luaFunctionId)
{
	if(env == NULL) return 0;
	return LuaJavaBridge::retainLuaFunctionById(luaFunctionId);
}

JNIEXPORT jint JNICALL Java_com_xishanju_defold_cocosext_LuaJavaBridge_releaseLuaFunction
	(JNIEnv *env, jclass cls, jint luaFunctionId)
{
	if(env == NULL) return 0;
	return LuaJavaBridge::releaseLuaFunctionById(luaFunctionId);
}

JNIEXPORT jobject JNICALL Java_com_xishanju_defold_cocosext_LuaJavaBridge_getMainActivity
	(JNIEnv *env, jclass cls)
{
	if(env == NULL) return 0;
	jobject obj = dmGraphics::GetNativeAndroidActivity();
	return obj;
}

#endif
