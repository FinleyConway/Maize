#pragma once

#include "Maize/Tools/PlatformDetection.h"

#ifndef NDEBUG
	#include <cassert>
	#define MZ_ASSERT_CORE(condition, message) \
		do { \
			if (!(condition)) { \
				MZ_LOG_CORE_ERROR("Assertion failed at {}:{} inside {}", __FILE__, __LINE__, __FUNCTION__); \
				MZ_LOG_CORE_ERROR("Message: {}", #message); \
				MZ_LOG_CORE_ERROR("Condition: {}", #condition); \
				MZ_DEBUGBREAK(); \
			} \
		} while(0)
	#define MZ_ASSERT_GAME(condition, message) \
		do { \
			if (!(condition)) { \
				MZ_LOG_ERROR("Assertion failed at {}:{} inside {}", __FILE__, __LINE__, __FUNCTION__); \
				MZ_LOG_ERROR("Message: {}", #message); \
				MZ_LOG_ERROR("Condition: {}", #condition); \
				MZ_DEBUGBREAK(); \
			} \
		} while(0)
	#else
		#define MZ_ASSERT_CORE(condition, message) do { } while(0)
		#define MZ_ASSERT_GAME(condition, message) do { } while(0)
#endif
