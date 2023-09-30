#include "mpch.h"
#include "Maize/Tilemap/CartesianGrid.h"

namespace Maize {

    const TilemapTile CartesianGrid::sc_InvalidTile = TilemapTile();

    CartesianGrid::CartesianGrid(sf::Vector2i defaultSize, int32_t resizeIncrements) :
            m_Grid(defaultSize.x * defaultSize.y),
            m_CurrentSize(defaultSize),
            m_ResizeIncrements(resizeIncrements)
    {
    }

    void CartesianGrid::SetTile(sf::Vector2i position, sf::Vector2i texCoords, bool flipX, bool flipY, float rotation, bool resize)
    {
        // adjust the position to account for the shifted origin
        int32_t adjustedX = position.x + m_CurrentSize.x / 2;
        int32_t adjustedY = position.y + m_CurrentSize.y / 2;

        // place tile if its within bounds
        if (adjustedX >= 0 && adjustedX < m_CurrentSize.x &&
            adjustedY >= 0 && adjustedY < m_CurrentSize.y)
        {
            m_Grid[m_CurrentSize.x * adjustedY + adjustedX] = TilemapTile(texCoords, flipX, flipY, rotation);
        }
        // resize grid and place tile
        else
        {
            if (resize)
            {
                ResizeGrid(position);
                adjustedX = position.x + m_CurrentSize.x / 2;
                adjustedY = position.y + m_CurrentSize.y / 2;
                m_Grid[m_CurrentSize.x * adjustedY + adjustedX] = TilemapTile(texCoords, flipX, flipY, rotation);
            }
        }
    }

    void CartesianGrid::RemoveTile(sf::Vector2i position)
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

    const TilemapTile& CartesianGrid::GetTile(sf::Vector2i position) const
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

    std::vector<std::pair<const TilemapTile&, sf::Vector2i>> CartesianGrid::GetSurroundingTiles(sf::Vector2i initPosition) const
    {
        std::vector<std::pair<const TilemapTile&, sf::Vector2i>> surrounding;

        static std::array<sf::Vector2i, 8> adjacentOffsets = {
                sf::Vector2i(-1, -1), sf::Vector2i(0, -1), sf::Vector2i(1, -1),
                sf::Vector2i(-1,  0),                            sf::Vector2i(1,  0),
                sf::Vector2i(-1,  1), sf::Vector2i(0,  1), sf::Vector2i(1,  1)
        };

        for (const auto& offset : adjacentOffsets)
        {
            int32_t x = initPosition.x + offset.x;
            int32_t y = initPosition.y + offset.y;

            surrounding.emplace_back(GetTile(sf::Vector2i(x, y)), sf::Vector2i(x, y));
        }

        return surrounding;
    }

    bool CartesianGrid::HasTile(sf::Vector2i position) const
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
        return m_Grid[m_CurrentSize.x * adjustedY + adjustedX].texCoords != sf::Vector2i(-1, -1);
    }

    bool CartesianGrid::IsValidPosition(sf::Vector2i position) const
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

    sf::Vector2i CartesianGrid::ConvertScreenToGrid(sf::Vector2f mousePosition, int32_t cellSizeX, int32_t cellSizeY)
    {
        return sf::Vector2i(
                static_cast<int32_t>(std::floor(mousePosition.x / static_cast<float>(cellSizeX))),
                static_cast<int32_t>(std::floor(mousePosition.y / static_cast<float>(cellSizeY)))
        );
    }

    sf::Vector2f CartesianGrid::ConvertGridToScreen(sf::Vector2i gridPosition, int32_t cellSizeX, int32_t cellSizeY)
    {
        return sf::Vector2f(
                static_cast<float>(gridPosition.x) * static_cast<float>(cellSizeX),
                static_cast<float>(gridPosition.y) * static_cast<float>(cellSizeY)
        );
    }

    void CartesianGrid::ResizeGrid(sf::Vector2i newTilePosition)
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
        m_CurrentSize = sf::Vector2i(newWidth, newHeight);
    }

} // Maize