#pragma once

#include <random>

namespace Maize {

	class Random
	{
	public:
		static int32_t Range(int32_t min, int32_t max);
		static float Range(float min, float max);
		static sf::Vector2i Range(sf::Vector2i min, sf::Vector2i max);
		static sf::Vector2f Range(sf::Vector2f min, sf::Vector2f max);
		static float Rotation();
		static float Value01();

	private:
		static std::mt19937 m_Generator;
	};

} // Maize