#if defined(DM_PLATFORM_ANDROID)

#include <dmsdk/sdk.h>

extern void cocosext_call_java_static_void_method(const char *clazz, const char *method, const char *signature, ...);

void plmext_onAppInit()
{
	cocosext_call_java_static_void_method("com.xishanju.plm.plmext.plmext", "init", "(Landroid/content/Context;)V", dmGraphics::GetNativeAndroidActivity());
}

#endif
