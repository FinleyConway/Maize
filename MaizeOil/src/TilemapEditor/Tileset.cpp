#include "Tileset.h"

namespace Maize {

	Tileset::Tileset()
	{
		// set default name and texture
		m_Texture = Texture::Create("Resources/Icons/default-image.png");
		SetName("Default");
	}

	bool Tileset::SetTexture(const std::string& textureFilePath)
	{
		auto texture = Texture::Create(textureFilePath);

		if (texture != nullptr)
		{
			m_Texture = std::move(texture);
			m_FilePath = textureFilePath;
			m_HasTexture = true;

			return true;
		}

		m_HasTexture = false;

		return false;
	}

	void Tileset::InitEmptyTiles()
	{
		sf::Vector2i numTiles = sf::Vector2i(m_Texture->GetWidth() / m_TileSize.x, m_Texture->GetHeight() / m_TileSize.y);

		for (int32_t x = 0; x < numTiles.x; x++)
		{
			for (int32_t y = 0; y < numTiles.y; y++)
			{
				int32_t tileIndex = x + y * numTiles.x;

				m_Tiles[tileIndex] = Tile(m_ID, sf::Vector2i(x, y), sf::Vector2i(x, y), m_TileSize);
			}
		}
	}

	void Tileset::AutoSetTiles(bool includeTransparent)
	{
		sf::Vector2i numTiles = sf::Vector2i(m_Texture->GetWidth() / m_TileSize.x, m_Texture->GetHeight() / m_TileSize.y);

		for (int32_t x = 0; x < numTiles.x; x++)
		{
			for (int32_t y = 0; y < numTiles.y; y++)
			{
				int32_t tileIndex = x + y * numTiles.x;

				sf::IntRect tileRect(x * m_TileSize.x, y * m_TileSize.y, m_TileSize.x, m_TileSize.y);

				bool isTransparent = m_Texture->IsRegionTransparent(tileRect.left, tileRect.top, tileRect.width, tileRect.height);

				if (includeTransparent || !isTransparent)
				{
					m_Tiles[tileIndex] = Tile(m_ID, sf::Vector2i(x, y), sf::Vector2i(x, y), m_TileSize);
				}
			}
		}
	}

	void Tileset::IncludeTile(int32_t x, int32_t y)
	{
		sf::Vector2i numTiles = sf::Vector2i(m_Texture->GetWidth() / m_TileSize.x, m_Texture->GetHeight() / m_TileSize.y);
		int32_t tileIndex = x + y * numTiles.x;

		if (auto it = m_Tiles.find(tileIndex); it != m_Tiles.end())
		{
			m_Tiles.erase(tileIndex);
		}
		else
		{
			m_Tiles[tileIndex] = Tile(m_ID, sf::Vector2i(x, y), sf::Vector2i(x, y), m_TileSize);
		}
	}

	Tile* Tileset::GetTile(int32_t index)
	{
		if (auto it = m_Tiles.find(index); it != m_Tiles.end())
		{
			return &m_Tiles.at(index);
		}

		return nullptr;
	}

	const Tile* Tileset::GetTile(int32_t index) const
	{
		if (auto it = m_Tiles.find(index); it != m_Tiles.end())
		{
			return &m_Tiles.at(index);
		}

		return nullptr;
	}

	bool Tileset::HasTile(int32_t index) const
	{
		if (auto it = m_Tiles.find(index); it != m_Tiles.end())
		{
			return true;
		}

		return false;
	}

	Tile* Tileset::FindTileByTilesetID(std::unordered_map<int32_t, Tileset>& tilesets, int32_t tilesetID, int32_t tileIndex)
	{
        if (auto it = tilesets.find(tilesetID); it != tilesets.end())
        {
            return tilesets.at(tilesetID).GetTile(tileIndex);
        }

		return nullptr;
	}

} // Maize