#pragma once

#include <Maize.h>

#include "../../CartesianGrid.h"

namespace Maize {

    class TilemapLayer
    {
    public:
        Point GetGridSize() { return m_Grid.GetSize(); }

        void PlaceTile(const TilemapTile& tile, Point gridPosition, bool flipX, bool flipY, float rotation, bool resize = true);
        void RemoveTile(Point gridPosition);
        void GetTileInfo(Point gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation);
        const TilemapTile& GetTile(Point gridPosition);
        void FillTiles(Point gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile);

    private:
        std::string m_Name;
        CartesianGrid m_Grid;
    };

} // Maize