#pragma once

#include <dmsdk/sdk.h>
#if defined(DM_PLATFORM_ANDROID)
#include <android/log.h>
#endif

typedef int LUA_FUNCTION;

#if defined(DM_PLATFORM_ANDROID)

#define COCOSEXT_LOG_TAG		"cocosext"
#define COCOSEXT_printf(msg...)	__android_log_print(ANDROID_LOG_INFO,COCOSEXT_LOG_TAG,msg)
#define COCOSEXT_warning(msg...)	__android_log_print(ANDROID_LOG_WARN,COCOSEXT_LOG_TAG,msg)
#define COCOSEXT_error(msg...)	__android_log_print(ANDROID_LOG_ERROR,COCOSEXT_LOG_TAG,msg)

#else

#define COCOSEXT_printf		printf
#define COCOSEXT_warning	printf
#define COCOSEXT_error		printf

#endif