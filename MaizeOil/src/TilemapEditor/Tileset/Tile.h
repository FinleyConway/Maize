#pragma once

#include <Maize.h>

namespace Maize {

    class Tile
    {
    public:
        Tile() = default;
        Tile(int32_t tilesetID, int32_t index, const Sprite& sprite, bool included = false) :
                m_TilesetID(tilesetID),
                m_Index(index),
                m_Sprite(sprite),
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

} // Maize