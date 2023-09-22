#include "mpch.h"
#include "Maize/Utils/Random.h"

namespace Maize {

	std::mt19937 Random::m_Generator;

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

	sf::Vector2i Random::Range(sf::Vector2i min, sf::Vector2i max)
	{
		int32_t x = Range(min.x, max.x);
		int32_t y = Range(min.y, max.y);

		return sf::Vector2i(x, y);
	}

	sf::Vector2f Random::Range(sf::Vector2f min, sf::Vector2f max)
	{
		float x = Range(min.x, max.x);
		float y = Range(min.y, max.y);

		return sf::Vector2f(x, y);
	}

	float Random::Rotation()
	{
		std::uniform_real_distribution<float> distribution(0.0f, 360.0f);

		return distribution(m_Generator);
	}

	float Random::Value01()
	{
		return Range(0.0f, 1.0f);
	}

}