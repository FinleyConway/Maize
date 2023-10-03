#include "mpch.h"
#include "Maize/Tilemap/TilemapLayer.h"

namespace Maize {

    void TilemapLayer::PlaceTile(const TilemapTile& tile, sf::Vector2i gridPosition, bool flipX, bool flipY, float rotation, sf::Vector2i size)
    {
        if (tile.IsValid())
        {
			m_Grid.InsertTile(gridPosition, true, tile.texCoords, flipX, flipY, rotation);
			m_GridRenderer.InsertTile(gridPosition, CreateQuad((sf::Vector2f)gridPosition, (sf::Vector2f)tile.texCoords, (sf::Vector2f)size), true);
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
            const TilemapTile& tile = m_Grid.GetTile(gridPosition);

            selectedTile = tile;
            flipX = tile.flipX;
            flipY = tile.flipY;
            rotation = tile.rotation;
        }
    }

    const TilemapTile& TilemapLayer::GetTile(sf::Vector2i gridPosition)
    {
        return m_Grid.GetTile(gridPosition);
    }

    void TilemapLayer::FillTiles(sf::Vector2i gridPosition, const TilemapTile& selectedTile, TilemapTile referenceTile, sf::Vector2i size)
    {
        if (!m_Grid.ContainsTile(gridPosition) && !m_GridRenderer.ContainsTile(gridPosition)) return;

        const TilemapTile& currentTile = m_Grid.GetTile(gridPosition);
        bool isReferenceTile = currentTile.texCoords == referenceTile.texCoords;
        bool isFilled = currentTile.texCoords != selectedTile.texCoords;

        if (isReferenceTile && isFilled)
        {
			m_Grid.InsertTile(gridPosition, false, selectedTile.texCoords, selectedTile.flipX, selectedTile.flipY, selectedTile.rotation);
			m_GridRenderer.InsertTile(gridPosition, CreateQuad((sf::Vector2f)gridPosition, (sf::Vector2f)selectedTile.texCoords, (sf::Vector2f)size), false);

            for (const auto& [neighbour, tilePosition] : m_Grid.GetSurroundingTiles(gridPosition))
            {
                FillTiles(tilePosition, selectedTile, referenceTile, size);
            }
        }
    }

	std::array<sf::Vertex, 4> TilemapLayer::CreateQuad(sf::Vector2f position, sf::Vector2f texCoord, sf::Vector2f size) const
	{
		std::array<sf::Vertex, 4> quad;

		quad[0].position = sf::Vector2f(position.x * size.x, position.y * size.y);
		quad[1].position = sf::Vector2f((position.x + 1) * size.x, position.y * size.y);
		quad[2].position = sf::Vector2f((position.x + 1) * size.x, (position.y + 1) * size.y);
		quad[3].position = sf::Vector2f(position.x * size.x, (position.y + 1) * size.y);

		quad[0].texCoords = sf::Vector2f(texCoord.x * size.x, texCoord.y * size.y);
		quad[1].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, texCoord.y * size.y);
		quad[2].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, (texCoord.y + 1) * size.y);
		quad[3].texCoords = sf::Vector2f(texCoord.x * size.x, (texCoord.y + 1) * size.y);

		return quad;
	}

} // Maize