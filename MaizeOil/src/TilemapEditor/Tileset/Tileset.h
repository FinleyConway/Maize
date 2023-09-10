#pragma once

#include <Maize.h>

#include <utility>

namespace Maize {

    class Tile2 // temp name until i replace previous tileset implementation
    {
    public:
        Tile2() = default;
        Tile2(int32_t tilesetID, int32_t index, Sprite sprite, bool included = false) :
        m_TilesetID(tilesetID),
        m_Index(index),
        m_Sprite(std::move(sprite)),
        m_Included(included)
        {}

        void SetIndex(int32_t index) { m_Index = index; }
        int32_t GetIndex() const { return m_Index; }

        int32_t GetTilesetID() const { return m_TilesetID; }
        Rect ImageRect() const { return m_Sprite.GetTextureRect(); }
        Sprite& GetSprite() { return m_Sprite; }

        bool IsIncluded() const { return m_Included; }
        void IsIncluded(bool included) { m_Included = included; }

    private:
        int32_t m_TilesetID = -1;
        int32_t m_Index = -1;
        Sprite m_Sprite;
        bool m_Included = false;
    };

    class Tileset
    {
    public:
        Tileset();

        void SetID(int32_t newID) {  m_ID = newID; }
        int32_t GetID() const { return m_ID; }

        void SetName(const std::string& name) { m_Name = name; }
        const std::string& GetName() const { return m_Name; }

        const std::string& GetFilePath() { return m_FilePath; }

        bool SetTexture(const std::string& textureFilePath);
        const Texture* GetTexture() const { return m_Texture.get(); }

        void SetTileSizeX(int32_t newSize) { m_TileSizeX = newSize; }
        int32_t GetTileSizeX() const { return m_TileSizeX; }

        void SetTileSizeY(int32_t newSize) { m_TileSizeY = newSize; }
        int32_t GetTileSizeY() const { return m_TileSizeY; }

        void InitEmptyTiles();
        void AutoSetTiles(bool includeTransparent);
        Tile2* GetTile(int32_t index);

        void Clear() { m_Tiles.clear(); }

    private:
        int32_t m_ID = 0;
        std::string m_Name;
        std::string m_FilePath;
        std::shared_ptr<Texture> m_Texture;
        int32_t m_TileSizeX = 16;
        int32_t m_TileSizeY = 16;

        std::vector<Tile2> m_Tiles;
    };

} // Maize