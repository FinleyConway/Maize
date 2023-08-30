#pragma once

#include <Maize.h>

namespace Maize {

    class Tile
    {
    public:
        Tile() = default;
        Tile(int32_t tilesetID, int32_t index, const Sprite& sprite) :
                m_TilesetID(tilesetID),
                m_Index(index),
                m_Sprite(sprite)
        {}

        int32_t GetIndex() const { return m_Index; }
        int32_t GetTilesetID() const { return m_TilesetID; }
        Rect ImageRect() const { return m_Sprite.GetTextureRect(); }
        Sprite& GetSprite() { return m_Sprite; }

    private:
        int32_t m_TilesetID = -1;
        int32_t m_Index = -1;
        Sprite m_Sprite;
    };

    class Tileset
    {
    public:
        Tileset(int32_t id, const std::string& name, const std::string& filePath, uint32_t cellSize) :
                m_TilesetName(name), m_FilePath(filePath), m_CellSize(cellSize), m_ID(id)
        {
            m_Texture = Texture::Create(filePath);

            if (m_Texture->IsValid())
            {
                m_Size = Point(m_Texture->GetWidth() / cellSize, m_Texture->GetHeight() / cellSize);
                m_Tiles.resize(m_Size.x * m_Size.y);

                for (int32_t x = 0; x < m_Size.x; x++)
                {
                    for (int32_t y = 0; y < m_Size.y; y++)
                    {
                        int32_t index = x + y * m_Size.x;
                        m_Tiles[index] = Tile(m_ID, index, Sprite(Rect(x * cellSize, y * cellSize, cellSize, cellSize), m_Texture.get(), PointF((float)m_CellSize / 2, (float)m_CellSize / 2)));
                    }
                }
            }
        }

        const std::string& GetName() const { return m_TilesetName; }
        const std::string& GetPath() const { return m_FilePath; }
        uint32_t GetCellSize() const { return m_CellSize; }
        int32_t GetID() const { return m_ID; }
        Point GetSize() const { return m_Size; }

        std::vector<Tile>& GetTiles() { return m_Tiles; }

        Tile& GetTile(int32_t index)
        {
            if (index >= 0 && index < m_Tiles.size())
            {
                return m_Tiles[index];
            }

            throw std::out_of_range("Invalid tile index");
        }

    private:
        std::string m_TilesetName;
        std::string m_FilePath;
        uint32_t m_CellSize = 0;
        int32_t m_ID = -1;

        std::vector<Tile> m_Tiles;

        std::shared_ptr<Texture> m_Texture;
        Point m_Size;
    };

} // Maize