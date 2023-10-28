#pragma once

#include <Maize.h>

#include "Tileset.h"
#include "TilemapEditorTile.h"

namespace Maize {

	class TilemapEditorHelper
	{
	public:
		static std::array<sf::Vertex, 4> CreateTile(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoords, bool flipX, bool flipY);

		static void PackTileset(std::unordered_map<int32_t, Tileset>& tilesets, TilemapComponent* tilemapComponent);
		static void UpdateMap(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent);

		static Tile* FindTileByTilesetID(std::unordered_map<int32_t, Tileset>& tilesets, int32_t tilesetID, int32_t tileIndex);
	};

} // Maize