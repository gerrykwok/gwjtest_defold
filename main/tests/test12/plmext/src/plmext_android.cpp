// include the Defold SDK
#include <dmsdk/sdk.h>

#if defined(DM_PLATFORM_ANDROID)

#include <android/log.h>
#include "plmext.h"

#define WEMU_LOG_TAG			"plmext"
#define WEMU_printf00(msg...)	__android_log_print(ANDROID_LOG_INFO,WEMU_LOG_TAG,msg)
#define WEMU_warning00(msg...)	__android_log_print(ANDROID_LOG_WARN,WEMU_LOG_TAG,msg)
#define WEMU_error00(msg...)	__android_log_print(ANDROID_LOG_ERROR,WEMU_LOG_TAG,msg)

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

void plm_take_photo(const char *localPath, int luaCallback)
{
}

void plm_get_local_picture(const char *localPath, int luaCallback)
{
	WEMU_printf00("plm_get_local_picture,1");
	AttachScope attachscope;
	WEMU_printf00("plm_get_local_picture,2");
	JNIEnv* env = attachscope.m_Env;
	WEMU_printf00("plm_get_local_picture,3");

	jclass cls = GetClass(env, "com.xishanju.plm.plmext.plmext");
	WEMU_printf00("plm_get_local_picture,4,cls=0x%x", cls);

	jmethodID dummy_method = env->GetStaticMethodID(cls, "getLocalPicture", "(Landroid/content/Context;Ljava/lang/String;I)V");
	WEMU_printf00("plm_get_local_picture,5,method=0x%x", dummy_method);
	jstring jpath = env->NewStringUTF(localPath);
	env->CallStaticVoidMethod(cls, dummy_method, dmGraphics::GetNativeAndroidActivity(), jpath, luaCallback);
	WEMU_printf00("plm_get_local_picture,6");
}

#endif
