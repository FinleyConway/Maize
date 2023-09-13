#include "TilemapLayer.h"

namespace Maize {

    void TilemapLayer::PlaceTile(const TilemapTile& tile, Point gridPosition, bool flipX, bool flipY, float rotation, bool resize)
    {
        if (tile.IsValid())
        {
            m_Grid.SetTile(gridPosition, tile.tilesetID, tile.index, flipX, flipX, rotation, resize);
        }
    }

    void TilemapLayer::RemoveTile(Point gridPosition)
    {
        if (m_Grid.HasTile(gridPosition))
        {
            m_Grid.RemoveTile(gridPosition);
        }
    }

    void TilemapLayer::GetTileInfo(Point gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation)
    {
        if (m_Grid.HasTile(gridPosition))
        {
            const TilemapTile& tile = m_Grid.GetTile(gridPosition);

            selectedTile = tile;
            flipX = tile.flipX;
            flipY = tile.flipY;
            rotation = tile.rotation;
        }
    }

    const TilemapTile& TilemapLayer::GetTile(Point gridPosition)
    {
        return m_Grid.GetTile(gridPosition);
    }

    void TilemapLayer::FillTiles(Point gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile)
    {
        if (!m_Grid.IsValidPosition(gridPosition)) return;

        const TilemapTile& currentTile = m_Grid.GetTile(gridPosition);
        bool isReferenceTile = currentTile.tilesetID == referenceTile.tilesetID && currentTile.index == referenceTile.index;
        bool isFilled = currentTile.tilesetID != selectedTile.tilesetID || currentTile.index != selectedTile.index;

        if (isReferenceTile && isFilled)
        {
            m_Grid.SetTile(gridPosition, selectedTile.tilesetID, selectedTile.index, selectedTile.flipX, selectedTile.flipY, selectedTile.rotation, false);

            for (const auto& [neighbour, tilePosition] : m_Grid.GetSurroundingTiles(gridPosition))
            {
                FillTiles(tilePosition, selectedTile, referenceTile);
            }
        }
    }

} // Maize