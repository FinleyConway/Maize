#pragma once

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

    class CartesianGrid
    {
    public:
        explicit CartesianGrid(sf::Vector2i defaultSize = sf::Vector2i(0, 0), int32_t resizeIncrements = 15);

        sf::Vector2i GetSize() const { return m_CurrentSize; }
        const std::vector<TilemapTile>& GetGrid() const { return m_Grid; }

        void SetTile(sf::Vector2i position, sf::Vector2i texCoords, bool flipX, bool flipY, float rotation, bool resize = true);
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
        sf::Vector2i m_CurrentSize;
        int32_t m_ResizeIncrements;
    };

} // Maize