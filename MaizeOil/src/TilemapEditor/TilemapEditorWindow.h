#pragma once

#include <Maize.h>

#include "TilesetWindowTab.h"
#include "TilemapWindowTab.h"

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

    class TilemapEditorWindow
    {
    public:
		using Tilesets = std::unordered_map<int32_t, Tileset>;
		using TilemapEditorGrid = std::vector<CartesianGrid<TilemapEditorTile>>;

		void AddComponent(TilemapComponent* tilemapComponent);
        void OnEvent(Event& e);
        void Window();

    private:
		TilemapComponent* m_TilemapComponent = nullptr;
		TilemapEditorGrid m_EditorGrids;
		Tilesets m_Tilesets;

        TilesetWindowTab m_TilesetWindow;
        TilemapWindowTab m_TilemapWindow;
    };

} // Maize