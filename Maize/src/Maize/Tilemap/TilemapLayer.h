#pragma once

#include "Maize/Utils/Grids/CartesianGrid.h"

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
        sf::Vector2i GetGridSize() const { return m_Grid.GridSize(); }

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }

        void PlaceTile(const TilemapTile& tile, sf::Vector2i gridPosition, bool flipX, bool flipY, float rotation);
        void RemoveTile(sf::Vector2i gridPosition);
        void GetTileInfo(sf::Vector2i gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation);
        const TilemapTile& GetTile(sf::Vector2i gridPosition) const;
        void FillTiles(sf::Vector2i gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile);

    private:
        std::string m_Name;
        CartesianGrid<TilemapTile> m_Grid;
    };

} // Maize