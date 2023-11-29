#include "TilemapEditorHelper.h"

namespace Maize {

	std::array<sf::Vertex, 4> TilemapEditorHelper::CreateTile(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoords, bool flipX, bool flipY)
	{
		std::array<sf::Vertex, 4> quad = Renderer::CreateQuad(position, rotation, size, texCoords);

		if (flipX)
		{
			std::swap(quad[0].position, quad[1].position);
			std::swap(quad[3].position, quad[2].position);
		}

		if (flipY)
		{
			std::swap(quad[0].position, quad[3].position);
			std::swap(quad[1].position, quad[2].position);
		}

		return quad;
	}

	void TilemapEditorHelper::PackTileset(std::unordered_map<int32_t, Tileset>& tilesets, TilemapComponent* tilemapComponent)
	{
		std::vector<TextureInfo> textureInfo;

		// add textures to temp array
		textureInfo.reserve(tilesets.size());
		for (auto& [tilesetID, tileset] : tilesets)
		{
			textureInfo.emplace_back(tileset.GetTexture()->GetTexture(), tilesetID);
		}

		// pack textures into one big texture
		auto results = TexturePacker::Pack(textureInfo, sf::Vector2u(1024, 1024));

		tilemapComponent->tilemapTexture = std::make_shared<Texture>(std::move(results.packedTexture));

		// offset all the tiles in the tileset of where they are in the packed texture
		for (auto& [tilesetID, tileset] : tilesets)
		{
			for (auto& [id, rect] : results.idToTextureRect)
			{
				if (id == tilesetID)
				{
					for (auto& [tileID, tile] : tileset.GetTiles())
					{
						sf::Vector2i texturePosition = rect.getPosition();

						tile.texCoords.x = tile.originalTexCoords.x + (texturePosition.x / tile.tileSize.x);
						tile.texCoords.y = tile.originalTexCoords.y + (texturePosition.y / tile.tileSize.y);
					}
				}
			}
		}
	}

	void TilemapEditorHelper::UpdateMap(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent*tilemapComponent)
	{
		PackTileset(tilesets, tilemapComponent);

		for (uint32_t i = 0; i < editorGrid.size(); i++)
		{
			auto& editorMap = editorGrid[i];
			auto& tilemap = tilemapComponent->tilemapLayers[i];

			sf::Vector2i halfSize = editorMap.grid.GridSize() / 2;

			for (int32_t y = -halfSize.y; y < halfSize.y; y++)
			{
				for (int32_t x = -halfSize.x; x < halfSize.x; x++)
				{
					const TilemapEditorTile* tile = editorMap.grid.GetTile(sf::Vector2i(x, y));

					if (tile == nullptr) continue;

					Tile* tilesetTile = FindTileByTilesetID(tilesets, tile->tilesetID, tile->tileIndex);

					if (tilesetTile == nullptr) continue;

					TilemapEditorTile newTile = *tile;
					newTile.texCoords = tilesetTile->texCoords;

					editorMap.grid.InsertTile(sf::Vector2i(x, y), true, newTile.tilesetID, newTile.tileIndex, newTile.texCoords, newTile.flipX, newTile.flipY, newTile.rotation);
					tilemap.InsertTile(sf::Vector2i(x, y), CreateTile(sf::Vector2f(x, y), newTile.rotation, (sf::Vector2f)tilesetTile->tileSize, (sf::Vector2f)newTile.texCoords, newTile.flipX, newTile.flipY), true);
				}
			}
		}
	}

	Tile* TilemapEditorHelper::FindTileByTilesetID(std::unordered_map<int32_t, Tileset>& tilesets, int32_t tilesetID, int32_t tileIndex)
	{
		if (tilesets.contains(tilesetID))
		{
			return tilesets.at(tilesetID).GetTile(tileIndex);
		}

		return nullptr;
	}

} // Maize