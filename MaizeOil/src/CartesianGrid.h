#pragma once

#include <Maize.h>
#include <array>
#include <cmath>

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
        explicit CartesianGrid(sf::Vector2u defaultSize = sf::Vector2u(0, 0), int32_t resizeIncrements = 15);

        sf::Vector2u GetSize() const { return m_CurrentSize; }
        const std::vector<TilemapTile>& GetGrid() const { return m_Grid; }

        void SetTile(sf::Vector2i position, int32_t tilesetID, int32_t index, bool flipX, bool flipY, float rotation, bool resize = true);
        void RemoveTile(sf::Vector2i position);
        const TilemapTile& GetTile(sf::Vector2i position) const;
        std::vector<std::pair<const TilemapTile&, sf::Vector2i>> GetSurroundingTiles(sf::Vector2i position) const;
        bool HasTile(sf::Vector2i position) const;
        bool IsValidPosition(sf::Vector2i position) const;

        static sf::Vector2f ConvertGridToScreen(sf::Vector2i gridPosition, int32_t cellSizeX, int32_t cellSizeY);
        static sf::Vector2i ConvertScreenToGrid(sf::Vector2f mousePosition, int32_t cellSizeX, int32_t cellSizeY);

    private:
        void ResizeGrid(sf::Vector2i newTilePosition);

        static const TilemapTile sc_InvalidTile;

        std::vector<TilemapTile> m_Grid;
        sf::Vector2u m_CurrentSize;
        int32_t m_ResizeIncrements;
    };

} // Maize