#include "CartesianGrid.h"

namespace Maize {

    const TilemapTile CartesianGrid::sc_InvalidTile = TilemapTile();

    CartesianGrid::CartesianGrid(Point defaultSize, int32_t resizeIncrements) :
            m_Grid(defaultSize.x * defaultSize.y),
            m_CurrentSize(defaultSize),
            m_ResizeIncrements(resizeIncrements)
    {
    }

    void CartesianGrid::SetTile(Point position, int32_t tilesetID, int32_t index, bool flipX, bool flipY, float rotation, bool resize)
    {
        // adjust the position to account for the shifted origin
        int32_t adjustedX = position.x + m_CurrentSize.x / 2;
        int32_t adjustedY = position.y + m_CurrentSize.y / 2;

        // place tile if its within bounds
        if (adjustedX >= 0 && adjustedX < m_CurrentSize.x &&
            adjustedY >= 0 && adjustedY < m_CurrentSize.y)
        {
            m_Grid[m_CurrentSize.x * adjustedY + adjustedX] = TilemapTile(tilesetID, index, flipX, flipY, rotation);
        }
            // resize grid and place tile
        else
        {
            if (resize)
            {
                ResizeGrid(position);
                adjustedX = position.x + m_CurrentSize.x / 2;
                adjustedY = position.y + m_CurrentSize.y / 2;
                m_Grid[m_CurrentSize.x * adjustedY + adjustedX] = TilemapTile(tilesetID, index, flipX, flipY, rotation);
            }
        }
    }

    void CartesianGrid::RemoveTile(Point position)
    {
        // adjust the position to account for the shifted origin
        int32_t adjustedX = position.x + m_CurrentSize.x / 2;
        int32_t adjustedY = position.y + m_CurrentSize.y / 2;

        // make sure the adjusted position is within valid range
        if (adjustedX >= 0 && adjustedX < m_CurrentSize.x && adjustedY >= 0 && adjustedY < m_CurrentSize.y)
        {
            m_Grid[m_CurrentSize.x * adjustedY + adjustedX] = sc_InvalidTile;
        }
    }

    const TilemapTile& CartesianGrid::GetTile(Point position) const
    {
        // adjust the position to account for the shifted origin
        int32_t adjustedX = position.x + m_CurrentSize.x / 2;
        int32_t adjustedY = position.y + m_CurrentSize.y / 2;

        // make sure the adjusted position is within valid range
        if (adjustedX < 0 || adjustedX >= m_CurrentSize.x || adjustedY < 0 || adjustedY >= m_CurrentSize.y)
        {
            // returns invalid tile
            return sc_InvalidTile;
        }

        return m_Grid[m_CurrentSize.x * adjustedY + adjustedX];
    }

    std::vector<std::pair<const TilemapTile&, Point>> CartesianGrid::GetSurroundingTiles(Point initPosition) const
    {
        std::vector<std::pair<const TilemapTile&, Point>> surrounding;

        static std::array<Point, 8> adjacentOffsets = {
                Point(-1, -1), Point(0, -1), Point(1, -1),
                Point(-1,  0),                     Point(1,  0),
                Point(-1,  1), Point(0,  1), Point(1,  1)
        };

        for (const auto& offset : adjacentOffsets)
        {
            int32_t x = initPosition.x + offset.x;
            int32_t y = initPosition.y + offset.y;

            surrounding.emplace_back(GetTile(Point(x, y)), Point(x, y));
        }

        return surrounding;
    }

    bool CartesianGrid::HasTile(Point position) const
    {
        // adjust the position to account for the shifted origin
        int32_t adjustedX = position.x + m_CurrentSize.x / 2;
        int32_t adjustedY = position.y + m_CurrentSize.y / 2;

        // make sure the adjusted position is within valid range
        if (adjustedX < 0 || adjustedX >= m_CurrentSize.x || adjustedY < 0 || adjustedY >= m_CurrentSize.y)
        {
            // position is out of bounds, no tile can exist here
            return false;
        }

        // check if the tile is valid
        return m_Grid[m_CurrentSize.x * adjustedY + adjustedX].index != -1;
    }

    bool CartesianGrid::IsValidPosition(Point position) const
    {
        // adjust the position to account for the shifted origin
        int32_t adjustedX = position.x + m_CurrentSize.x / 2;
        int32_t adjustedY = position.y + m_CurrentSize.y / 2;

        // make sure the adjusted position is within valid range
        if (adjustedX < 0 || adjustedX >= m_CurrentSize.x || adjustedY < 0 || adjustedY >= m_CurrentSize.y)
        {
            // position is out of bounds, no tile can exist here
            return false;
        }

        return true;
    }

    Point CartesianGrid::ConvertScreenToGrid(PointF mousePosition, int32_t cellSizeX, int32_t cellSizeY)
    {
        return Point(
                static_cast<int32_t>(std::floor(mousePosition.x / static_cast<float>(cellSizeX))),
                static_cast<int32_t>(std::floor(mousePosition.y / static_cast<float>(cellSizeY)))
        );
    }

    PointF CartesianGrid::ConvertGridToScreen(Point gridPosition, int32_t cellSizeX, int32_t cellSizeY)
    {
        return PointF(
                static_cast<float>(gridPosition.x) * static_cast<float>(cellSizeX),
                static_cast<float>(gridPosition.y) * static_cast<float>(cellSizeY)
        );
    }

    void CartesianGrid::ResizeGrid(Point newTilePosition)
    {
        int32_t adjustedX = newTilePosition.x + m_CurrentSize.x / 2;
        int32_t adjustedY = newTilePosition.y + m_CurrentSize.y / 2;

        int32_t offsetX = 0;
        int32_t offsetY = 0;

        if (adjustedX < 0)
        {
            offsetX = (-adjustedX / m_ResizeIncrements + 1) * m_ResizeIncrements;
        }
        else if (adjustedX >= m_CurrentSize.x)
        {
            offsetX = ((adjustedX - m_CurrentSize.x) / m_ResizeIncrements + 1) * m_ResizeIncrements;
        }

        if (adjustedY < 0)
        {
            offsetY = (-adjustedY / m_ResizeIncrements + 1) * m_ResizeIncrements;
        }
        else if (adjustedY >= m_CurrentSize.y)
        {
            offsetY = ((adjustedY - m_CurrentSize.y) / m_ResizeIncrements + 1) * m_ResizeIncrements;
        }

        int32_t newWidth = m_CurrentSize.x + offsetX * 2;
        int32_t newHeight = m_CurrentSize.y + offsetY * 2;

        std::vector<TilemapTile> newGrid(newWidth * newHeight, sc_InvalidTile);

        for (int32_t y = 0; y < m_CurrentSize.y; ++y)
        {
            for (int32_t x = 0; x < m_CurrentSize.x; ++x)
            {
                int32_t new_x = x + offsetX;
                int32_t new_y = y + offsetY;
                newGrid[new_y * newWidth + new_x] = m_Grid[y * m_CurrentSize.x + x];
            }
        }

        m_Grid = std::move(newGrid);
        m_CurrentSize = Point(newWidth, newHeight);
    }

} // Maize