// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

#include "../plmext.h"
#include "../plmext_luastack.h"
#include "plmext_android.h"

static JNIEnv* Attach()
{
	JNIEnv* env;
	JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
	vm->AttachCurrentThread(&env, NULL);
	return env;
}

static bool Detach(JNIEnv* env)
{
	bool exception = (bool) env->ExceptionCheck();
	env->ExceptionClear();
	JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
	vm->DetachCurrentThread();
	return !exception;
}

namespace {
	struct AttachScope
	{
		JNIEnv* m_Env;
		AttachScope() : m_Env(Attach())
		{
		}
		~AttachScope()
		{
			Detach(m_Env);
		}
	};
}

static jclass GetClass(JNIEnv* env, const char* classname)
{
	jclass activity_class = env->FindClass("android/app/NativeActivity");
	jmethodID get_class_loader = env->GetMethodID(activity_class,"getClassLoader", "()Ljava/lang/ClassLoader;");
	jobject cls = env->CallObjectMethod(dmGraphics::GetNativeAndroidActivity(), get_class_loader);
	jclass class_loader = env->FindClass("java/lang/ClassLoader");
	jmethodID find_class = env->GetMethodID(class_loader, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

	jstring str_class_name = env->NewStringUTF(classname);
	jclass outcls = (jclass)env->CallObjectMethod(cls, find_class, str_class_name);
	env->DeleteLocalRef(str_class_name);
	return outcls;
}

void plm_get_photo(int fromCamera, const char *localPath, int width, int height, int luaCallback)
{
//	Java_com_xishanju_plm_plmext_TakePhoto_ndkNotifyResult(NULL, NULL, 0, 0);//为了能在编译时能被编译进去
	
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;

	jclass cls = GetClass(env, "com.xishanju.plm.plmext.plmext");

	jmethodID dummy_method = env->GetStaticMethodID(cls, "getPhoto", "(Landroid/content/Context;ILjava/lang/String;III)V");
	jstring jpath = env->NewStringUTF(localPath);
	env->CallStaticVoidMethod(cls, dummy_method, dmGraphics::GetNativeAndroidActivity(), fromCamera, jpath, width, height, luaCallback);
}

//JNIEXPORT void JNICALL Java_com_xishanju_plm_plmext_TakePhoto_ndkNotifyResult(JNIEnv *env, jclass clz, jint luaCallback, jstring res)
//{
//	if(env == NULL)
//	{
//		return;
//	}
//
//	const char *value_ = env->GetStringUTFChars(res, 0);
//	LuaStack::getInstance()->pushString(value_);
//	LuaStack::getInstance()->executeFunctionByHandler(luaCallback, 1);
//	env->ReleaseStringUTFChars(res, value_);
//
//	LuaStack::getInstance()->removeScriptHandler(luaCallback);
//}

#endif
