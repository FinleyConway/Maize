#pragma once

namespace Maize {

	class Tile
	{
	public:
		Tile() = default;
		Tile(int32_t tilesetID, sf::Vector2i position) : tilesetID(tilesetID), texCoords(position) {}

		int32_t tilesetID = -1;
        sf::Vector2i texCoords;
	};

} // Maize