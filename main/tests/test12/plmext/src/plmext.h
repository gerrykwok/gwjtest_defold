#pragma once

#include <dmsdk/sdk.h>
#if defined(DM_PLATFORM_ANDROID)
#include <android/log.h>
#endif

typedef int LUA_FUNCTION;

#if defined(DM_PLATFORM_ANDROID)

#define PLMEXT_LOG_TAG			"plmext"
#define PLMEXT_printf(msg...)	__android_log_print(ANDROID_LOG_INFO,PLMEXT_LOG_TAG,msg)
#define PLMEXT_warning(msg...)	__android_log_print(ANDROID_LOG_WARN,PLMEXT_LOG_TAG,msg)
#define PLMEXT_error(msg...)	__android_log_print(ANDROID_LOG_ERROR,PLMEXT_LOG_TAG,msg)

#else

#define PLMEXT_printf	printf
#define PLMEXT_warning	printf
#define PLMEXT_error	printf

#endif