#pragma once

jobject ext_JSONObjectFromLuaTable(JNIEnv *env, lua_State *L, int index);
void ext_callLuaCallbackInAndroid(JNIEnv *env, jint callback, jstring value, bool unregister);
