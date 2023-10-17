#include "mpch.h"
#include "Maize/Tilemap/TilemapLayer.h"
#include "Maize/Renderer/Renderer.h"

namespace Maize {

    void TilemapLayer::PlaceTile(const TilemapTile& tile, sf::Vector2i gridPosition, bool flipX, bool flipY, float rotation, sf::Vector2i size)
    {
        if (tile.IsValid())
        {
			m_Grid.InsertTile(gridPosition, true, tile.tilesetID, tile.tileIndex, tile.texCoords, flipX, flipY, rotation);
			m_GridRenderer.InsertTile(gridPosition, Renderer::CreateQuad((sf::Vector2f)gridPosition, 0, (sf::Vector2f)size, (sf::Vector2f)tile.texCoords), true);
        }
    }

	void TilemapLayer::RemoveTile(sf::Vector2i gridPosition)
    {
        if (m_Grid.ContainsTile(gridPosition) && m_GridRenderer.ContainsTile(gridPosition))
        {
            m_Grid.RemoveTile(gridPosition);
			m_GridRenderer.RemoveTile(gridPosition);
        }
    }

    void TilemapLayer::GetTileInfo(sf::Vector2i gridPosition, TilemapTile& selectedTile, bool& flipX, bool& flipY, float& rotation)
    {
        if (m_Grid.ContainsTile(gridPosition) && m_GridRenderer.ContainsTile(gridPosition))
        {
            const TilemapTile* tile = m_Grid.GetTile(gridPosition);

            selectedTile = *tile;
            flipX = tile->flipX;
            flipY = tile->flipY;
            rotation = tile->rotation;
        }
    }

    const TilemapTile* TilemapLayer::GetTile(sf::Vector2i gridPosition) const
    {
        return m_Grid.GetTile(gridPosition);
    }

    void TilemapLayer::FillTiles(sf::Vector2i gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile, sf::Vector2i size)
    {
        if (!m_Grid.ContainsTile(gridPosition) && !m_GridRenderer.ContainsTile(gridPosition)) return;

        const TilemapTile* currentTile = m_Grid.GetTile(gridPosition);

        if (currentTile == nullptr) return;

        bool isReferenceTile = currentTile->texCoords == referenceTile.texCoords;
        bool isFilled = currentTile->texCoords != selectedTile.texCoords;

        if (isReferenceTile && isFilled)
        {
			m_Grid.InsertTile(gridPosition, false, selectedTile.tilesetID, selectedTile.tileIndex, selectedTile.texCoords, selectedTile.flipX, selectedTile.flipY, selectedTile.rotation);
			m_GridRenderer.InsertTile(gridPosition, Renderer::CreateQuad((sf::Vector2f)gridPosition, 0, (sf::Vector2f)size, (sf::Vector2f)selectedTile.texCoords), false);

            for (const auto& [neighbour, tilePosition] : m_Grid.GetSurroundingTiles(gridPosition))
            {
                FillTiles(tilePosition, selectedTile, referenceTile, size);
            }
        }
    }

} // Maize