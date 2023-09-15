#pragma once

#include <Maize.h>

#include "Tile.h"

namespace Maize {

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
        const Texture* GetTexture() const { return m_Texture.get(); }
        bool HasTexture() const { return m_HasTexture; }

        void SetTileSizeX(int32_t newSize) { m_TileSizeX = newSize; }
        int32_t GetTileSizeX() const { return m_TileSizeX; }

        void SetTileSizeY(int32_t newSize) { m_TileSizeY = newSize; }
        int32_t GetTileSizeY() const { return m_TileSizeY; }

        void InitEmptyTiles();
        void Clear() { m_Tiles.clear(); }

        void AutoSetTiles(bool includeTransparent);

        Tile* GetTile(int32_t index);

        static Tile* FindTileByTilesetID(std::vector<Tileset>& tilesets, int32_t tilesetID, int32_t tileIndex);

    private:
        int32_t m_ID = 0;
        std::string m_Name;
        std::string m_FilePath;

        std::shared_ptr<Texture> m_Texture;
        bool m_HasTexture = false;

        int32_t m_TileSizeX = 8;
        int32_t m_TileSizeY = 8;

        std::vector<Tile> m_Tiles;
    };

} // Maize