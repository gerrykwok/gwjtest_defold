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

class AttachScope
{
public:
	JNIEnv* m_Env;
	AttachScope() : m_Env(Attach())
	{
	}
	~AttachScope()
	{
		Detach(m_Env);
	}
};

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

void cocosext_call_java_static_void_method(const char *clazz, const char *method, const char *signature, ...)
{
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;

	jclass cls = GetClass(env, clazz);
	jmethodID dummy_method = env->GetStaticMethodID(cls, method, signature);
	va_list ap;
	va_start(ap, signature);
	env->CallStaticVoidMethodV(cls, dummy_method, ap);
	va_end(ap);
}

void cocosext_set_activity_to_java()
{
	cocos2d::JniHelper::setJavaVM(dmGraphics::GetNativeAndroidJavaVM());

//	AttachScope attachscope;
//	JNIEnv* env = attachscope.m_Env;
//
//	jclass cls = GetClass(env, "com.xishanju.defold.cocosext.GlobalContext");
//
//	jmethodID dummy_method = env->GetStaticMethodID(cls, "setActivity", "(Landroid/content/Context;)V");
//	env->CallStaticVoidMethod(cls, dummy_method, dmGraphics::GetNativeAndroidActivity());
	cocosext_call_java_static_void_method("com.xishanju.defold.cocosext.GlobalContext", "setActivity", "(Landroid/content/Context;)V", dmGraphics::GetNativeAndroidActivity());

	//为了保证jni函数能编译进so
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_callLuaFunctionWithString(NULL, NULL, 0, NULL);
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_callLuaGlobalFunctionWithString(NULL, NULL, NULL, NULL);
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_retainLuaFunction(NULL, NULL, 0);
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_releaseLuaFunction(NULL, NULL, 0);
	Java_com_xishanju_defold_cocosext_LuaJavaBridge_getMainActivity(NULL, NULL);
}

#endif
