#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Maize {

	class Log
	{
	 public:
		static void Initialize();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetGameLogger() { return s_GameLogger; }

	 private:
		inline static std::shared_ptr<spdlog::logger> s_CoreLogger;
		inline static std::shared_ptr<spdlog::logger> s_GameLogger;
	};

} // Maize

// core log macros
#define MZ_LOG_CORE_TRACE(...)    ::Maize::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MZ_LOG_CORE_INFO(...)     ::Maize::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MZ_LOG_CORE_WARN(...)     ::Maize::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MZ_LOG_CORE_ERROR(...)    ::Maize::Log::GetCoreLogger()->error(__VA_ARGS__)

// game log macros
#define MZ_LOG_TRACE(...)         ::Maize::Log::GetGameLogger()->trace(__VA_ARGS__)
#define MZ_LOG_INFO(...)          ::Maize::Log::GetGameLogger()->info(__VA_ARGS__)
#define MZ_LOG_WARN(...)          ::Maize::Log::GetGameLogger()->warn(__VA_ARGS__)
#define MZ_LOG_ERROR(...)         ::Maize::Log::GetGameLogger()->error(__VA_ARGS__)