#pragma once

#if defined(_WIN32)
	#ifdef _WIN64
		#define MZ_PLATFORM_WINDOWS
	#else
		#error "x86 builds not supported!"
	#endif
#elif defined(__linux__)
	#define MZ_PLATFORM_LINUX
#else
	#error "Unknown platform"
#endif


#ifndef NDEBUG
	#if defined(MZ_PLATFORM_WINDOWS)
		#define MZ_DEBUGBREAK() __debugbreak()
	#elif defined(MZ_PLATFORM_LINUX)
		#include <csignal>
		#define MZ_DEBUGBREAK() raise(SIGTRAP)
	#endif
#else
	#define MZ_DEBUGBREAK()
#endif