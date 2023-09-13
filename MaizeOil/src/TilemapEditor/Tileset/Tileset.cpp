#include "Tileset.h"

namespace Maize {

    Tileset::Tileset()
    {
        // set default name and texture
        m_Texture = Texture::Create("Resources/Icons/default-image.png");
        SetName("Default");
    }

    bool Tileset::SetTexture(const std::string &textureFilePath)
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
        int32_t sizeX = (int32_t)m_Texture->GetWidth() / m_TileSizeX;
        int32_t sizeY = (int32_t)m_Texture->GetHeight() / m_TileSizeY;
        m_Tiles.resize(sizeX * sizeY);

        for (int32_t x = 0; x < sizeX; x++)
        {
            for (int32_t y = 0; y < sizeY; y++)
            {
                int32_t index = x + y * sizeX;
                Sprite sprite(Rect(x * m_TileSizeX, y * m_TileSizeY, m_TileSizeX, m_TileSizeY), m_Texture.get(), PointF((float)m_TileSizeX / 2.0f, (float)m_TileSizeY / 2.0f));

                m_Tiles[index] = Tile(m_ID, index, sprite, false);
            }
        }
    }

    void Tileset::AutoSetTiles(bool includeTransparent)
    {
        int32_t sizeX = (int32_t)m_Texture->GetWidth() / m_TileSizeX;
        int32_t sizeY = (int32_t)m_Texture->GetHeight() / m_TileSizeY;
        m_Tiles.resize(sizeX * sizeY);

        for (int32_t x = 0; x < sizeX; x++)
        {
            for (int32_t y = 0; y < sizeY; y++)
            {
                int32_t regionX = x * m_TileSizeX;
                int32_t regionY = y * m_TileSizeY;

                if (includeTransparent)
                {
                    int32_t index = x + y * sizeX;
                    Sprite sprite(Rect(x * m_TileSizeX, y * m_TileSizeY, m_TileSizeX, m_TileSizeY), m_Texture.get(), PointF((float)m_TileSizeX / 2.0f, (float)m_TileSizeY / 2.0f));

                    m_Tiles[index] = Tile(m_ID, index, sprite, true);

                }
                else if (!m_Texture->IsRegionTransparent(regionX, regionY, m_TileSizeX, m_TileSizeY))
                {
                    int32_t index = x + y * sizeX;
                    Sprite sprite(Rect(x * m_TileSizeX, y * m_TileSizeY, m_TileSizeX, m_TileSizeY), m_Texture.get(), PointF((float)m_TileSizeX / 2.0f, (float)m_TileSizeY / 2.0f));

                    m_Tiles[index] = Tile(m_ID, index, sprite, true);
                }
            }
        }
    }

    Tile *Tileset::GetTile(int32_t index)
    {
        if (index >= 0 && index < m_Tiles.size())
        {
            return &m_Tiles[index];
        }

        return nullptr;
    }

} // Maize