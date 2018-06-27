#ifndef _PLMEXT_ANDROID_H_
#define _PLMEXT_ANDROID_H_

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_xishanju_plm_plmext_TakePhoto_ndkNotifyResult(JNIEnv *env, jclass clz, jint luaCallback, jstring res);

#ifdef __cplusplus
}
#endif

#endif
