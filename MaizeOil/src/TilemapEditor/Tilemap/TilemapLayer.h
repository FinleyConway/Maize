#pragma once

#include <Maize.h>

#include "../../CartesianGrid.h"

namespace Maize {

    class TilemapLayer
    {
    public:
        Point GetGridSize() { return m_Grid.GetSize(); }

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() { return m_Name; }

        void PlaceTile(const TilemapTile& tile, Point gridPosition, bool flipX, bool flipY, float rotation, bool resize = true);
        void RemoveTile(Point gridPosition);
        void GetTileInfo(Point gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation);
        const TilemapTile& GetTile(Point gridPosition);
        void FillTiles(Point gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile);

    private:
        friend class TilemapEditorWindow;

        std::string m_Name;
        CartesianGrid m_Grid;
    };

} // Maize