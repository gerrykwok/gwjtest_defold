#pragma once

#include <dmsdk/sdk.h>
#if defined(DM_PLATFORM_ANDROID)
#include <android/log.h>
#endif

#if defined(DM_PLATFORM_ANDROID)

#define LOG_TAG				"cocosext"
#define LOG_printf(msg...)	__android_log_print(ANDROID_LOG_INFO,LOG_TAG,msg)
#define LOG_warning(msg...)	__android_log_print(ANDROID_LOG_WARN,LOG_TAG,msg)
#define LOG_error(msg...)	__android_log_print(ANDROID_LOG_ERROR,LOG_TAG,msg)

#else

#define LOG_printf		printf
#define LOG_warning		printf
#define LOG_error		printf

#endif
