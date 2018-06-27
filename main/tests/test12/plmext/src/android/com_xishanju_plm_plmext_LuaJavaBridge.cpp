#if defined(DM_PLATFORM_ANDROID)

#include "com_xishanju_plm_plmext_LuaJavaBridge.h"
//#include "../plmext_luastack.h"

#include "CCLuaJavaBridge.h"

/*
void LuaJavaBridge_ensureCompileIn()
{
	Java_com_xishanju_plm_plmext_LuaJavaBridge_callLuaFunctionWithString(NULL, NULL, 0, NULL);
	Java_com_xishanju_plm_plmext_LuaJavaBridge_releaseLuaFunction(NULL, NULL, 0);
}

JNIEXPORT jint JNICALL Java_com_xishanju_plm_plmext_LuaJavaBridge_callLuaFunctionWithString(JNIEnv *env, jclass clz, jint luaFunc, jstring res)
{
	int ret;
	if(env == NULL) return 0;
	const char *value_ = env->GetStringUTFChars(res, 0);
	LuaStack::getInstance()->pushString(value_);
	ret = LuaStack::getInstance()->executeFunctionByHandler(luaFunc, 1);
	env->ReleaseStringUTFChars(res, value_);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_xishanju_plm_plmext_LuaJavaBridge_releaseLuaFunction(JNIEnv *env, jclass clz, jint luaFunc)
{
	if(env == NULL) return 0;
	LuaStack::getInstance()->removeScriptHandler(luaFunc);
	return 0;
}
*/

JNIEXPORT jint JNICALL Java_com_xishanju_plm_plmext_LuaJavaBridge_callLuaFunctionWithString
	(JNIEnv *env, jclass cls, jint functionId, jstring value)
{
	const char *value_ = env->GetStringUTFChars(value, 0);
	int ret = LuaJavaBridge::callLuaFunctionById(functionId, value_);
	env->ReleaseStringUTFChars(value, value_);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_xishanju_plm_plmext_LuaJavaBridge_callLuaGlobalFunctionWithString
	(JNIEnv *env, jclass cls, jstring luaFunctionName, jstring value)
{
	const char *luaFunctionName_ = env->GetStringUTFChars(luaFunctionName, 0);
	const char *value_ = env->GetStringUTFChars(value, 0);
	int ret = LuaJavaBridge::callLuaGlobalFunction(luaFunctionName_, value_);
	env->ReleaseStringUTFChars(luaFunctionName, luaFunctionName_);
	env->ReleaseStringUTFChars(value, value_);
	return ret;
}

JNIEXPORT jint JNICALL Java_com_xishanju_plm_plmext_LuaJavaBridge_retainLuaFunction
	(JNIEnv *env, jclass cls, jint luaFunctionId)
{
	return LuaJavaBridge::retainLuaFunctionById(luaFunctionId);
}

JNIEXPORT jint JNICALL Java_com_xishanju_plm_plmext_LuaJavaBridge_releaseLuaFunction
	(JNIEnv *env, jclass cls, jint luaFunctionId)
{
	return LuaJavaBridge::releaseLuaFunctionById(luaFunctionId);
}

#endif
