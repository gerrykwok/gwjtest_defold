#if defined(DM_PLATFORM_ANDROID)

#include <math.h>
#include "../luacallback.h"
#include "luajavaoc_android.h"

//static JNIEnv* Attach()
//{
//	JNIEnv* env;
//	JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
//	vm->AttachCurrentThread(&env, NULL);
//	return env;
//}
//
//static bool Detach(JNIEnv* env)
//{
//	bool exception = (bool) env->ExceptionCheck();
//	env->ExceptionClear();
//	JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
//	vm->DetachCurrentThread();
//	return !exception;
//}

class AttachScope
{
public:
	JNIEnv* m_Env;
	bool m_attached;
	AttachScope()
	{
		m_attached = false;
		JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
		jint status = vm->GetEnv((void**)&m_Env, JNI_VERSION_1_4);
		switch(status)
		{
		case JNI_EDETACHED:
			vm->AttachCurrentThread(&m_Env, NULL);
//			dmLogInfo("version attached:0x%x", m_Env->GetVersion());
			m_attached = true;
			break;
		case JNI_EVERSION:
			dmLogError("GetEnv not support JNI version 1.4");
			break;
		default:
			if(status != JNI_OK)
				dmLogError("GetEnv error:%d", status);
			break;
		}
	}
	~AttachScope()
	{
		if(m_Env)
		{
			m_Env->ExceptionCheck();
			m_Env->ExceptionClear();
		}
		if(m_attached)
		{
			JavaVM* vm = dmGraphics::GetNativeAndroidJavaVM();
			vm->DetachCurrentThread();
		}
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

void ext_callJavaStaticMethodV(const char *clazz, const char *method, const char *signature, ...)
{
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;
	jclass cls = GetClass(env, clazz);
	if(cls == NULL)
	{
		std::string str = (std::string)"failed to find class " + clazz;
		dmLogError(str.c_str());
		return;
	}
	jmethodID dummy_method = env->GetStaticMethodID(cls, method, signature);
	if(dummy_method == NULL)
	{
		std::string str = (std::string)"failed to find method " + clazz + "." + method + " " + signature;
		dmLogError(str.c_str());
		return;
	}

	const char *retType = strstr(signature, ")");
	if(retType) retType++;
	dmLogInfo("rettype=%s", retType);

	va_list args;
	va_start(args, signature);
	if(strcmp(retType, "Z"))
		env->CallStaticBooleanMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "B"))
		env->CallStaticByteMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "C"))
		env->CallStaticCharMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "S"))
		env->CallStaticShortMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "I"))
		env->CallStaticIntMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "J"))
		env->CallStaticLongMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "F"))
		env->CallStaticFloatMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "D"))
		env->CallStaticDoubleMethodV(cls, dummy_method, args);
	else if(strcmp(retType, "V"))
		env->CallStaticVoidMethodV(cls, dummy_method, args);
	else
		env->CallStaticObjectMethodV(cls, dummy_method, args);
	va_end(args);
}

static jobject newJavaObject(JNIEnv *env, const char *className)
{
	jclass cls = env->FindClass(className);
	if(cls == NULL) return NULL;
	jmethodID ctorID = env->GetMethodID(cls, "<init>", "()V");
	if(ctorID == NULL)
	{
		env->DeleteLocalRef(cls);
		return NULL;
	}
	jobject obj = env->NewObject(cls, ctorID);
	if(cls) env->DeleteLocalRef(cls);
	return obj;
}

static jobject put_to_json(JNIEnv *env, jobject jsonObj, const char *name, jvalue value, const char *valueType)
{
//	dmLogInfo("gwjgwj,put to json,name=%s,valuetype=%s", name, valueType);
	if(strcmp(valueType, "D") == 0)
	{
		char buf[256];
		sprintf(buf, "%f", value.d);
		if(strcmp(buf, "inf") == 0)
		{
			dmLogWarning("gwjgwj,found infinity,convert to string inf");
			valueType = "Ljava/lang/Object;";
			value.l = stoJstring(env, "inf");
		}
	}
	jclass cls = env->GetObjectClass(jsonObj);
	std::string signature = (std::string)"(Ljava/lang/String;" + valueType + ")Lorg/json/JSONObject;";
	jmethodID methodPut = env->GetMethodID(cls, "put", signature.c_str());
	if(methodPut == NULL)
	{
		dmLogError("gwjgwj,cannot get method of json.put %s", signature.c_str());
		return NULL;
	}
	jvalue args[2];
	args[0].l = stoJstring(env, name);
	args[1] = value;
	jobject ret = env->CallObjectMethodA(jsonObj, methodPut, args);
	env->DeleteLocalRef(cls);//?
	return ret;
}

jobject ext_JSONObjectFromLuaTable(JNIEnv *env, lua_State *L, int index)
{
	jobject ret = newJavaObject(env, "org/json/JSONObject");
	jobject sub = NULL;
	jvalue value;
	int top = lua_gettop(L);
	std::string sKey, sValue;
	int i;
	char buf[256];
	lua_Number fValue, abs;
	int funcId;

	int type = top > 0 ? lua_type(L, index) : LUA_TNIL;
//	dmLogInfo("gwjgwj,type in index is %d top=%d", type, top);
	if(type != LUA_TTABLE)
	{
		return ret;
	}
	
	lua_pushnil(L);  /* first key */
	while(lua_next(L, index-1) != 0)
	{
		/* 'key' is at index -2 and 'value' at index -1 */
		switch(lua_type(L, -2))
		{
		case LUA_TNIL:
			sKey = "nil";
			break;
		case LUA_TBOOLEAN:
			sKey = lua_toboolean(L, -2) ? "true" : "false";
			break;
		case LUA_TNUMBER:
			sprintf(buf, "%d", (int)lua_tonumber(L, -2));
			sKey = buf;
			break;
		case LUA_TSTRING:
			sKey = lua_tostring(L, -2);
			break;
		default:
			sKey = "";
			break;
		}

		if(sKey.length() > 0) switch(lua_type(L, -1))
		{
		case LUA_TBOOLEAN:
			value.z = lua_toboolean(L, -1);
			put_to_json(env, ret, sKey.c_str(), value, "Z");
			break;
		case LUA_TNUMBER:
			fValue = lua_tonumber(L, -1);
			abs = fValue >= 0 ? fValue : -fValue;
			if(abs - floor(abs) == 0)
			{
				value.i = fValue;
				put_to_json(env, ret, sKey.c_str(), value, "I");
			}
			else
			{
				value.d = fValue;
				put_to_json(env, ret, sKey.c_str(), value, "D");
			}
			break;
		case LUA_TSTRING:
			value.l = stoJstring(env, lua_tostring(L, -1));
			put_to_json(env, ret, sKey.c_str(), value, "Ljava/lang/Object;");
			break;
		case LUA_TFUNCTION:
			funcId = ext_registerLuaCallback(L, -1);
			value.i = funcId;
			put_to_json(env, ret, sKey.c_str(), value, "I");
			break;
		case LUA_TTABLE:
			sub = ext_JSONObjectFromLuaTable(env, L, -1);
			value.l = sub;
			put_to_json(env, ret, sKey.c_str(), value, "Ljava/lang/Object;");
			break;
		default: break;
		}

//		dmLogInfo("%s - %s", lua_typename(L, lua_type(L, -2)), lua_typename(L, lua_type(L, -1)));
//		dmLogInfo("\"%s\":%s", sKey.c_str(), sValue.c_str());

		lua_pop(L, 1);  /* removes 'value'; keeps 'key' for next iteration */
	}

	assert(top + 0 == lua_gettop(L));
	return ret;
}

std::string ext_callNativeStaticMethodBase(const char *clazz, const char *method, lua_State *L, int idxParam, bool *ok)
{
	AttachScope attachscope;
	JNIEnv* env = attachscope.m_Env;
	*ok = true;

	jobject json = ext_JSONObjectFromLuaTable(env, L, idxParam);

	jclass cls = GetClass(env, clazz);
	if(cls == NULL)
	{
		*ok = false;
		std::string str = (std::string)"failed to find class " + clazz;
		dmLogError(str.c_str());
		return str;
	}
	const char *signature = "(Landroid/content/Context;Lorg/json/JSONObject;)Ljava/lang/String;";
	jmethodID dummy_method = env->GetStaticMethodID(cls, method, signature);
	if(dummy_method == NULL)
	{
		*ok = false;
		std::string str = (std::string)"failed to find method " + clazz + "." + method + " " + signature;
		dmLogError(str.c_str());
		return str;
	}
	jstring retjs = (jstring)env->CallStaticObjectMethod(cls, dummy_method, dmGraphics::GetNativeAndroidActivity(), json);

	const char *stringBuff = env->GetStringUTFChars(retjs, 0);
	std::string retStr = stringBuff;
	env->ReleaseStringUTFChars(retjs, stringBuff);
	return retStr;
}

void ext_callLuaCallbackInAndroid(JNIEnv *env, jint callback, jstring value, bool unregister)
{
	if(callback <= 0) return;
	jstring value2 = (jstring)env->NewGlobalRef(value);
	auto func = [=](){
		JavaVM *vm = dmGraphics::GetNativeAndroidJavaVM();
		JNIEnv *env2;
		vm->AttachCurrentThread(&env2, NULL);
		const char *value_ = env2->GetStringUTFChars(value2, 0);

		ext_invokeLuaCallbackWithString(callback, value_);
		if(unregister)
			ext_unregisterLuaCallback(callback);

		env2->ReleaseStringUTFChars(value2, value_);
		env2->DeleteGlobalRef(value2);
		vm->DetachCurrentThread();
	};
	ext_performInUpdateThread(func);
}

int ext_gettimeofday(long *sec, long *usec)
{
	int ret;
	struct timeval now;
	ret = gettimeofday(&now, NULL);
	if(ret == 0)
	{
		if(sec) *sec = now.tv_sec;
		if(usec) *usec = now.tv_usec;
	}
	return ret;
}

#endif
