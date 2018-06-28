// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

#include "../cocosext.h"
#include "../CCLuaStack.h"
#include "cocosext_android.h"
#include "JniHelper.h"
#include "com_xishanju_defold_cocosext_LuaJavaBridge.h"

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

void cocosext_set_activity_to_java()
{
	JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
	cocos2d::JniHelper::setJavaVM(vm);
	
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;

	jclass cls = GetClass(env, "com.xishanju.defold.cocosext.GlobalContext");

	jmethodID dummy_method = env->GetStaticMethodID(cls, "setActivity", "(Landroid/content/Context;)V");
	env->CallStaticVoidMethod(cls, dummy_method, dmGraphics::GetNativeAndroidActivity());

	//为了保证jni函数能编译进so
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_callLuaFunctionWithString(NULL, NULL, 0, NULL);
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_callLuaGlobalFunctionWithString(NULL, NULL, NULL, NULL);
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_retainLuaFunction(NULL, NULL, 0);
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_releaseLuaFunction(NULL, NULL, 0);
}

#endif
