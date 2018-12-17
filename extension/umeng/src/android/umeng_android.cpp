#if defined(DM_PLATFORM_ANDROID)

#include "../umeng.h"

#define UMENG_CLASS				"com.xishanju.plm.umeng.UmengUtil"

void umeng_onAppInit()
{
	std::string params = (std::string)"{\"appkey\":\"" + UMENG_APPKEY_ANDROID + "\",\"channel\":\"" + UMENG_CHANNEL + "\"}";
	bool ok;
	ext_callJavaStaticMethod(UMENG_CLASS, "init", params.c_str(), &ok);
	//程序启动时，不会触发activate app，而安卓的onResume会触发，所以在这里调用一下resume
//	ext_performWithDelay(8, [=]() {
//		dmLogInfo("gwjgwj,call umeng.resume");
//		bool ok;
		ext_callJavaStaticMethod(UMENG_CLASS, "onActivityEvent", "resume", &ok);
//	});
}

void umeng_onAppActivate()
{
	bool ok;
	ext_callJavaStaticMethod(UMENG_CLASS, "onActivityEvent", "resume", &ok);
}

void umeng_onAppDeactivate()
{
	bool ok;
	ext_callJavaStaticMethod(UMENG_CLASS, "onActivityEvent", "pause", &ok);
}

int umeng_sendEvent(lua_State *L)
{
	return ext_callNativeStaticMethod(UMENG_CLASS, "sendEvent", L, -1);
}

#endif