#pragma once

#include <Maize.h>

namespace Maize {

	struct TilemapEditorTile
	{
		int32_t tilesetID = -1;
		int32_t tileIndex = -1;
		sf::Vector2i texCoords = sf::Vector2i(-1, -1);

		bool flipX = false;
		bool flipY = false;
		float rotation = 0.0f;

		bool IsValid() const
		{
			return texCoords != sf::Vector2i(-1, -1) || tilesetID == -1 || tileIndex == -1;
		}
	};

	struct TilemapEditorLayer
	{
		std::string layerName;
		CartesianGrid<TilemapEditorTile> grid;
	};

}