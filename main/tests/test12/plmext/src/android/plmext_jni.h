#include <jni.h>

#ifndef PLMEXT_JNI_H
#define PLMEXT_JNI_H

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_xishanju_plm_plmext_TakePhoto_ndkCallLuaWithString(JNIEnv *env, jclass clz, jint luaCallback, jstring res);

#ifdef __cplusplus
}
#endif

#endif
