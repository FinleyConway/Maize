#pragma once

#include "Maize/Renderer/Texture.h"

namespace Maize {

	using TileIndex = int32_t;

	struct Tile
	{
		int32_t tilesetID = -1;
		sf::Vector2i texCoords;
	};

	class Tileset
	{
	public:
		Tileset();

		void SetID(int32_t newID) {  m_ID = newID; }
		int32_t GetID() const { return m_ID; }

		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }

		const std::string& GetFilePath() const { return m_FilePath; }

		bool SetTexture(const std::string& textureFilePath);
		std::shared_ptr<Texture> GetTexture() const { return m_Texture; }
		bool HasTexture() const { return m_HasTexture; }

		void SetTileSize(sf::Vector2i newTileSize) { m_TileSize = newTileSize; }
		sf::Vector2i GetTileSize() const { return m_TileSize; }

		void InitEmptyTiles();
		void AutoSetTiles(bool includeTransparent);
		void IncludeTile(int32_t x, int32_t y);

		Tile* GetTile(int32_t index);
		const Tile* GetTile(int32_t index) const;

		bool HasTile(int32_t index) const;

		std::unordered_map<TileIndex, Tile>& GetTiles() { return m_Tiles; }
		void Clear() { m_Tiles.clear(); }

		static Tile* FindTileByTilesetID(std::unordered_map<int32_t, Tileset>& tilesets, int32_t tilesetID, int32_t tileIndex);

	private:
		int32_t m_ID = -1;
		std::string m_Name;
		std::string m_FilePath;

		std::shared_ptr<Texture> m_Texture; // temp, reference a pre-existing texture in the future
		bool m_HasTexture = false;

		sf::Vector2i m_TileSize = sf::Vector2i(8, 8);

		std::unordered_map<TileIndex, Tile> m_Tiles;
	};

} // Maize