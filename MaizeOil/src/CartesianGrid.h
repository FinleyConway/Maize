#pragma once

#include <Maize.h>
#include <array>

namespace Maize {

    struct TilemapTile
    {
        int32_t tilesetID = -1; // from where
        int32_t index = -1; // where it is
        bool flipX = false; // flip sprite
        bool flipY = false;
        float rotation = 0.0f;

        bool IsValid() const
        {
            return tilesetID != -1 && index != -1;
        }
    };

    class CartesianGrid
    {
    public:
        explicit CartesianGrid(Point defaultSize = Point(0, 0), int32_t resizeIncrements = 15);

        Point GetSize() const { return m_CurrentSize; }
        const std::vector<TilemapTile>& GetGrid() const { return m_Grid; }

        void SetTile(Point position, int32_t tilesetID, int32_t index, bool flipX, bool flipY, float rotation, bool resize = true);
        void RemoveTile(Point position);
        const TilemapTile& GetTile(Point position) const;
        std::vector<std::pair<const TilemapTile&, Point>> GetSurroundingTiles(Point position) const;
        bool HasTile(Point position) const;
        bool IsValidPosition(Point position) const;

        static PointF ConvertGridToScreen(Point gridPosition, int32_t cellSizeX, int32_t cellSizeY);
        static Point ConvertScreenToGrid(PointF mousePosition, int32_t cellSizeX, int32_t cellSizeY);

    private:
        void ResizeGrid(Point newTilePosition);

        static const TilemapTile sc_InvalidTile;

        std::vector<TilemapTile> m_Grid;
        Point m_CurrentSize;
        int32_t m_ResizeIncrements;
    };

} // Maize