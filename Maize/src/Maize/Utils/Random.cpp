#include "mpch.h"
#include "Maize/Utils/Random.h"

namespace Maize {

	int32_t Random::Range(int32_t min, int32_t max)
	{
		std::uniform_int_distribution<int32_t> distribution(min, max);
		return distribution(m_Generator);
	}

	float Random::Range(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(m_Generator);
	}

	float Random::Rotation()
	{
		std::uniform_real_distribution<float> distribution(0.0f, 360.0f);
		return distribution(m_Generator);
	}

	float Random::Value01()
	{
		std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
		return distribution(m_Generator);
	}

} // Maize