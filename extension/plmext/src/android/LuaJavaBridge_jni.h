#include <jni.h>

#define DEF_JNI_FUNCTION(name) Java_com_xishanju_plm_plmext_LuaJavaBridge_##name

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(callLuaFunctionWithString)(JNIEnv *, jclass, jint, jstring);
JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(callLuaGlobalFunctionWithString)(JNIEnv *, jclass, jstring, jstring);
JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(retainLuaFunction)(JNIEnv *, jclass, jint);
JNIEXPORT jint JNICALL DEF_JNI_FUNCTION(releaseLuaFunction)(JNIEnv *, jclass, jint);
JNIEXPORT void JNICALL DEF_JNI_FUNCTION(runOnGLThread)(JNIEnv *, jclass, jobject);

#ifdef __cplusplus
}
#endif
