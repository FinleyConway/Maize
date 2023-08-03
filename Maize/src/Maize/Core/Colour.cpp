#include "Maize/Core/Colour.h"

namespace Maize {

	const Colour Colour::black = Colour(0, 0, 0);
	const Colour Colour::blue = Colour(0, 0, 255);
	const Colour Colour::clear = Colour(0, 0, 0, 0);
	const Colour Colour::cyan = Colour(0, 255, 255);
	const Colour Colour::grey = Colour(127, 127, 127);
	const Colour Colour::green = Colour(0, 255, 0);
	const Colour Colour::magenta = Colour(255, 0, 255);
	const Colour Colour::red = Colour(255, 0, 0);
	const Colour Colour::white = Colour(255, 255, 255);

	Colour::Colour(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : r(r), g(g), b(b), a(a)
	{
	}

}