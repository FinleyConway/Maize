#pragma once

#include "Maize/Utils/Grids/CartesianGrid.h"
#include "Maize/Utils/Grids/VertexGrid.h"

namespace Maize {

    struct TilemapTile
    {
        sf::Vector2i texCoords = sf::Vector2i(-1, -1);
        bool flipX = false;
        bool flipY = false;
        float rotation = 0.0f;

        bool IsValid() const
        {
            return texCoords != sf::Vector2i(-1, -1);
        }
    };

    class TilemapLayer
    {
    public:
        const CartesianGrid<TilemapTile>& GetGrid() const { return m_Grid; }
		const VertexGrid& GetGridRenderer() const { return m_GridRenderer; }

        sf::Vector2i GetGridSize() const { return m_Grid.GridSize(); }

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }

        void PlaceTile(const TilemapTile& tile, sf::Vector2i gridPosition, bool flipX, bool flipY, float rotation, sf::Vector2i size);
        void RemoveTile(sf::Vector2i gridPosition);
        void GetTileInfo(sf::Vector2i gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation);
        const TilemapTile& GetTile(sf::Vector2i gridPosition) const;
        void FillTiles(sf::Vector2i gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile, sf::Vector2i size);

	private:
		std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, sf::Vector2f texCoord, sf::Vector2f size) const;

	private:
        std::string m_Name;
        CartesianGrid<TilemapTile> m_Grid = CartesianGrid<TilemapTile>(sf::Vector2i(50, 50));
		VertexGrid m_GridRenderer  = VertexGrid(sf::Vector2i(50, 50));
    };

} // Maize