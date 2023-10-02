#include "mpch.h"
#include "Maize/Tilemap/TilemapLayer.h"

namespace Maize {

    void TilemapLayer::PlaceTile(const TilemapTile& tile, sf::Vector2i gridPosition, bool flipX, bool flipY, float rotation)
    {
        if (tile.IsValid())
        {
            m_Grid.EmplaceTile(gridPosition, true, tile.texCoords, flipX, flipY, rotation);
        }
    }

    void TilemapLayer::RemoveTile(sf::Vector2i gridPosition)
    {
        if (m_Grid.ContainsTile(gridPosition))
        {
            m_Grid.RemoveTile(gridPosition);
        }
    }

    void TilemapLayer::GetTileInfo(sf::Vector2i gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation)
    {
        if (m_Grid.ContainsTile(gridPosition))
        {
            const TilemapTile& tile = m_Grid.GetTile(gridPosition);

            selectedTile = tile;
            flipX = tile.flipX;
            flipY = tile.flipY;
            rotation = tile.rotation;
        }
    }

    const TilemapTile& TilemapLayer::GetTile(sf::Vector2i gridPosition) const
    {
        return m_Grid.GetTile(gridPosition);
    }

    void TilemapLayer::FillTiles(sf::Vector2i gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile)
    {
        if (!m_Grid.ContainsTile(gridPosition)) return;

        const TilemapTile& currentTile = m_Grid.GetTile(gridPosition);
        bool isReferenceTile = currentTile.texCoords == referenceTile.texCoords;
        bool isFilled = currentTile.texCoords != selectedTile.texCoords;

        if (isReferenceTile && isFilled)
        {
            m_Grid.EmplaceTile(gridPosition, false, selectedTile.texCoords, selectedTile.flipX, selectedTile.flipY, selectedTile.rotation);

            for (const auto& [neighbour, tilePosition] : m_Grid.GetSurroundingTiles(gridPosition))
            {
                FillTiles(tilePosition, selectedTile, referenceTile);
            }
        }
    }

} // Maize