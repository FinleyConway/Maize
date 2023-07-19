#pragma once

#include <SDL.h>

namespace Maize {

	struct Colour
	{
	public:
		Colour() = default;
		explicit Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

		uint8_t r = 255;
		uint8_t g = 255;
		uint8_t b = 255;
		uint8_t a = 255;

		const static Colour black;
		const static Colour blue;
		const static Colour clear;
		const static Colour cyan;
		const static Colour grey;
		const static Colour green;
		const static Colour magenta;
		const static Colour red;
		const static Colour white;
	};

}