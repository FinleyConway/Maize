#pragma once

#include "Maize/Utils/Grids/CartesianGrid.h"
#include "Maize/Utils/Grids/VertexGrid.h"

namespace Maize {

    struct TilemapTile
    {
		int32_t tilesetID = -1;
		int32_t tileIndex = -1;
		sf::Vector2i texCoords = sf::Vector2i(-1, -1);

        bool flipX = false; // to be removed
        bool flipY = false; // to be removed
        float rotation = 0.0f; // to be removed

		// float orientation = 0.0f;

        bool IsValid() const
        {
            return texCoords != sf::Vector2i(-1, -1) || tilesetID == -1 || tileIndex == -1;
        }
    };

    class TilemapLayer
    {
    public:
        CartesianGrid<TilemapTile>& GetGrid() { return m_Grid; }
		const VertexGrid& GetGridRenderer() const { return m_GridRenderer; }

        sf::Vector2i GetGridSize() const { return m_Grid.GridSize(); }

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }

        void PlaceTile(const TilemapTile& tile, sf::Vector2i gridPosition, bool flipX, bool flipY, float rotation, sf::Vector2i size);
        void RemoveTile(sf::Vector2i gridPosition);
        void GetTileInfo(sf::Vector2i gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation);
        const TilemapTile& GetTile(sf::Vector2i gridPosition);
        void FillTiles(sf::Vector2i gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile, sf::Vector2i size);

	private:
        std::string m_Name;
        CartesianGrid<TilemapTile> m_Grid;
		VertexGrid m_GridRenderer;
    };

} // Maize