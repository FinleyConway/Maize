#pragma once

#include <random>

namespace Maize {

	class Random
	{
	public:
		static int32_t Range(int32_t min, int32_t max);
		static float Range(float min, float max);
		static float Rotation();
		static float Value01();

	private:
		inline static std::mt19937 m_Generator = std::mt19937(std::random_device{}());
	};

} // Maize