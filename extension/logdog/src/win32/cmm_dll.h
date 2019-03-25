//////////////////////////////////////////////////////////////////////////
#pragma once

#ifndef GWJ_SYMBOL
	#ifdef _WIN32
		#ifdef DLL_EXPORTS
			#define GWJ_SYMBOL					__declspec(dllexport)
		#else
			#define GWJ_SYMBOL					__declspec(dllimport)
		#endif
	#else
		#define GWJ_SYMBOL
	#endif
#endif

#ifndef GWJ_API
#ifdef _WIN32
#define GWJ_API						__stdcall
#else
#define GWJ_API
#endif
#endif
