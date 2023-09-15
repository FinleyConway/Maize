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
        int32_t numTilesX = m_Texture->GetWidth() / m_TileSizeX;
        int32_t numTilesY = m_Texture->GetHeight() / m_TileSizeY;

        m_Tiles.resize(numTilesX * numTilesY);

        for (int32_t x = 0; x < numTilesX; x++)
        {
            for (int32_t y = 0; y < numTilesY; y++)
            {
                // calculate the region coordinates for the current tile
                int32_t regionX = x * m_TileSizeX;
                int32_t regionY = y * m_TileSizeY;

                int32_t tileIndex = x + y * numTilesX;

                Sprite sprite(Rect(regionX, regionY, m_TileSizeX, m_TileSizeY), m_Texture.get(), PointF(static_cast<float>(m_TileSizeX) / 2.0f, static_cast<float>(m_TileSizeY) / 2.0f));
                m_Tiles[tileIndex] = Tile(m_ID, tileIndex, sprite, false);
            }
        }
    }

    void Tileset::AutoSetTiles(bool includeTransparent)
    {
        int32_t numTilesX = m_Texture->GetWidth() / m_TileSizeX;
        int32_t numTilesY = m_Texture->GetHeight() / m_TileSizeY;

        m_Tiles.resize(numTilesX * numTilesY);

        for (int32_t x = 0; x < numTilesX; x++)
        {
            for (int32_t y = 0; y < numTilesY; y++)
            {
                // calculate the region coordinates for the current tile
                int32_t regionX = x * m_TileSizeX;
                int32_t regionY = y * m_TileSizeY;

                // check if we should include transparent tiles or if the current region is not transparent
                if (includeTransparent || !m_Texture->IsRegionTransparent(regionX, regionY, m_TileSizeY, m_TileSizeY))
                {
                    int32_t tileIndex = x + y * numTilesX;

                    Sprite sprite(Rect(regionX, regionY, m_TileSizeX, m_TileSizeY), m_Texture.get(), PointF(static_cast<float>(m_TileSizeX) / 2.0f, static_cast<float>(m_TileSizeY) / 2.0f));
                    m_Tiles[tileIndex] = Tile(m_ID, tileIndex, sprite, true);
                }
            }
        }
    }

    Tile* Tileset::GetTile(int32_t index)
    {
        // check if it's within the grid
        if (index >= 0 && index < m_Tiles.size())
        {
            return &m_Tiles[index];
        }

        return nullptr;
    }

    Tile *Tileset::FindTileByTilesetID(std::vector<Tileset> &tilesets, int32_t tilesetID, int32_t tileIndex)
    {
        for (auto& tileset : tilesets)
        {
            if (tileset.GetID() == tilesetID)
            {
                return tileset.GetTile(tileIndex);
            }
        }
        return nullptr;
    }

} // Maize