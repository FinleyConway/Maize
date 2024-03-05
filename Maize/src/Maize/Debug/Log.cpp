#include "mpch.h"
#include "Maize/Debug/Log.h"

namespace Maize {

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");

		s_CoreLogger = spdlog::stderr_color_mt("MAIZE");
		s_CoreLogger->set_level(spdlog::level::trace);

		s_GameLogger = spdlog::stderr_color_mt("GAME");
		s_GameLogger->set_level(spdlog::level::trace);
	}

} // Maize