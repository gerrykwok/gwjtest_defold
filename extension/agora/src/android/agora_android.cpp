#if defined(DM_PLATFORM_ANDROID)

#include "../agora.h"
#include "../common/MyAgora.h"

void agora_onAppInit()
{
	bool ok;
	ext_callJavaStaticMethod("com.xishanju.plm.agora.AgoraUtil", "init", "", &ok);
}

extern "C" JNIEXPORT void JNICALL Java_com_xishanju_plm_agora_AgoraUtil_nativeInit(JNIEnv *env, jclass clz)
{
	agora_initAgora();
}

#endif