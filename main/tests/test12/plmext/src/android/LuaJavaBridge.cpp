#include "com_xishanju_plm_plmext_LuaJavaBridge.h"
#include "../plmext_luastack.h"

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
