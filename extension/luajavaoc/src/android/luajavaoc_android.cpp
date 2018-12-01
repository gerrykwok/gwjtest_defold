#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>
#include "../luacallback.h"

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

static jstring stoJstring(JNIEnv* env, const char *pat)
{
	//定义java String类 strClass
	jclass strClass = env->FindClass("java/lang/String");
	//获取java String类方法String(byte[],String)的构造器,用于将本地byte[]数组转换为一个新String
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));//建立byte数组
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);//将char* 转换为byte数组
	jstring encoding = env->NewStringUTF("utf-8");// 设置String, 保存语言类型,用于byte数组转换至String时的参数
	jstring ret = (jstring)env->NewObject(strClass, ctorID, bytes, encoding);//将byte数组转换为java String,并输出
	env->DeleteLocalRef(strClass);
	env->DeleteLocalRef(bytes);
	env->DeleteLocalRef(encoding);
	return ret;
}

std::string ext_call_java_static_void_method(const char *clazz, const char *method, const char *signature, ...)
{
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;

	jclass cls = GetClass(env, clazz);
	if(cls == NULL) return (std::string)"failed to find class " + clazz;
	jmethodID dummy_method = env->GetStaticMethodID(cls, method, signature);
	if(dummy_method == NULL) return (std::string)"failed to find method " + clazz + "." + method + " " + signature;
	va_list ap;
	va_start(ap, signature);
	env->CallStaticVoidMethodV(cls, dummy_method, ap);
	va_end(ap);
	return "success";
}

std::string ext_callJavaStaticMethod(const char *clazz, const char *method, const char *params, bool *ok)
{
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;
	*ok = true;

	jclass cls = GetClass(env, clazz);
	if(cls == NULL)
	{
		*ok = false;
		std::string str = (std::string)"failed to find class " + clazz;
		dmLogError(str.c_str());
		return str;
	}
	const char *signature = "(Landroid/content/Context;Ljava/lang/String;)Ljava/lang/String;";
	jmethodID dummy_method = env->GetStaticMethodID(cls, method, signature);
	if(dummy_method == NULL)
	{
		*ok = false;
		std::string str = (std::string)"failed to find method " + clazz + "." + method + " " + signature;
		dmLogError(str.c_str());
		return str;
	}
	jstring str = stoJstring(env, params);
	jstring retjs = (jstring)env->CallStaticObjectMethod(cls, dummy_method, dmGraphics::GetNativeAndroidActivity(), str);
	env->DeleteLocalRef(str);

	const char *stringBuff = env->GetStringUTFChars(retjs, 0);
	std::string retStr = stringBuff;
	env->ReleaseStringUTFChars(retjs, stringBuff);
	return retStr;
}

#endif
