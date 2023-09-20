#pragma once

#include <Maize.h>

#include "../../CartesianGrid.h"

namespace Maize {

    class TilemapLayer
    {
    public:
        const CartesianGrid& GetGrid() const { return m_Grid; }
        sf::Vector2u GetGridSize() const { return m_Grid.GetSize(); }

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }

        void PlaceTile(const TilemapTile& tile, sf::Vector2i gridPosition, bool flipX, bool flipY, float rotation, bool resize = true);
        void RemoveTile(sf::Vector2i gridPosition);
        void GetTileInfo(sf::Vector2i gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation) const;
        const TilemapTile& GetTile(sf::Vector2i gridPosition) const;
        void FillTiles(sf::Vector2i gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile);

    private:
        std::string m_Name;
        CartesianGrid m_Grid;
    };

} // Maize