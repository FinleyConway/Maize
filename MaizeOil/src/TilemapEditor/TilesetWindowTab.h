#pragma once

#include <Maize.h>

namespace Maize {

    class TilesetWindowTab
    {
    public:
		TilesetWindowTab();

		void AddComponent(TilemapComponent* tilemapComponent)
		{
			m_TilemapComponent = tilemapComponent;
		}
        void Window();

    private:
        int32_t CreateID() // temp, will create a better id
        {
            static int32_t id = 0;
            return id++;
        }

        Tileset& AddTileset();
        void RemoveTileset(int32_t tilesetID);

        void SelectTileset();
        void ShowCurrentTileset();
        void TextureSelector();
        void SetAutomaticTiles();
        void HandleTilesetTiles();

		void PackTileset()
		{
			Maize::TexturePacker pack;
			std::vector<Maize::TextureInfo> textureInfo;

			// add textures to temp array
			textureInfo.reserve(m_TilemapComponent->tilesets.size());
			for (auto& [tilesetID, tileset] : m_TilemapComponent->tilesets)
			{
				textureInfo.emplace_back(*tileset.GetTexture(), tilesetID);
			}

			// pack textures into one big texture
			auto results = pack.Pack(textureInfo, sf::Vector2u(1024, 1024));

			m_TilemapComponent->texture = std::make_shared<sf::Texture>(results.packedTexture);

			// offset all the tiles in the tileset of where they are in the packed texture
			for (auto& [tilesetID, tileset] : m_TilemapComponent->tilesets)
			{
				for (auto& [id, rect] : results.textureInfo)
				{
					if (id == tilesetID)
					{
						for (auto& [tileID, tile] : tileset.GetTiles())
						{
                            sf::Vector2i texturePosition = rect.getPosition();

                            tile.texCoords.x = tile.originalTexCoords.x + (texturePosition.x / m_TilemapComponent->tileSizeX);
                            tile.texCoords.y = tile.originalTexCoords.y + (texturePosition.y / m_TilemapComponent->tileSizeY);
						}
					}
				}
			}
		}
		void UpdateMap()
		{
			PackTileset();

			for (auto& layer : m_TilemapComponent->layers)
			{
				sf::Vector2i halfSize = layer.GetGridSize() / 2;

				for (int32_t y = -halfSize.y; y < halfSize.y; y++)
				{
					for (int32_t x = -halfSize.x; x < halfSize.x; x++)
					{
						const TilemapTile* tile = layer.GetGrid().GetTile(sf::Vector2i(x, y));

						if (tile == nullptr) continue;

						Tile* tilesetTile = Tileset::FindTileByTilesetID(m_TilemapComponent->tilesets, tile->tilesetID, tile->tileIndex);

						if (tilesetTile == nullptr) continue;

						TilemapTile newTile = *tile;
						newTile.texCoords = tilesetTile->texCoords;

						sf::Vector2i tileSize = sf::Vector2i(m_TilemapComponent->tileSizeX, m_TilemapComponent->tileSizeY); // temp;

						layer.PlaceTile(newTile, sf::Vector2i(x, y), newTile.flipX, newTile.flipY, newTile.rotation, tileSize);
					}
				}
			}
		}

    private:
        TilemapComponent* m_TilemapComponent = nullptr;
        Tileset* m_SelectedTileset = nullptr;
		float m_TilesetZoomFactor = 4.0f;

        std::shared_ptr<Texture> m_IconAdd, m_IconRemove;
    };

} // Maize