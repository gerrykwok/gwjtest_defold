#include <stdlib.h>
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

#include "../plmext.h"
#include "plmext_jni.h"
#include "plmext_android.h"
#include "../plmext_luastack.h"

JNIEXPORT void JNICALL Java_com_xishanju_plm_plmext_TakePhoto_ndkCallLuaWithString(JNIEnv *env, jclass clz, jint luaCallback, jstring res)
{
	if(env == NULL)
	{
		return;
	}

	const char *value_ = env->GetStringUTFChars(res, 0);
	LuaStack::getInstance()->pushString(value_);
	LuaStack::getInstance()->executeFunctionByHandler(luaCallback, 1);
	env->ReleaseStringUTFChars(res, value_);
}

#endif
