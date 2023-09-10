#pragma once

#include <SFML/Graphics/Sprite.hpp>

#include "Maize/Renderer/Texture.h"
#include "Maize/Renderer/Colour.h"
#include "Maize/Math/PointF.h"
#include "Maize/Math/Rect.h"

namespace Maize {

    struct Sprite
    {
    public:
        Sprite() = default;
        Sprite(const Rect& rect, const Texture* texture, PointF pivot = PointF(0, 0)) :
                m_Pivot(pivot)
        {
            if (texture != nullptr)
            {
                SetPivot(pivot);
                m_Sprite.setTextureRect(rect);
                m_Sprite.setTexture(*texture);
            }
        }

        void SetPosition(PointF position) { m_Sprite.setPosition(position); }
        PointF GetPosition() const { return m_Sprite.getPosition(); }

        void SetColour(Colour colour) { m_Sprite.setColor(colour); }
        Colour GetColour() const { return m_Sprite.getColor(); }

        Rect GetTextureRect() const { return m_Sprite.getTextureRect(); }

        void SetPivot(PointF pivot) { m_Sprite.setOrigin(pivot); }
        PointF GetPivot() const { return m_Pivot; }

        void SetAngle(float angle) { m_Sprite.setRotation(angle); }
        float GetAngle() const { return m_Sprite.getRotation(); }

        void FlipX(bool flip)
        {
            m_FlipX = flip;
            float scaleX = flip ? -1.0f : 1.0f;
            m_Sprite.setScale(scaleX, m_Sprite.getScale().y);
        }

        void FlipY(bool flip)
        {
            m_FlipY = flip;
            float scaleY = flip ? -1.0f : 1.0f;
            m_Sprite.setScale(m_Sprite.getScale().x, scaleY);
        }

        bool IsFlippedX() const { return m_FlipX; }
        bool IsFlippedY() const { return m_FlipY; }

        operator const sf::Sprite& () const { return m_Sprite; }

    private:
        PointF m_Pivot = PointF(0, 0);
        sf::Sprite m_Sprite;
        bool m_FlipX = false;
        bool m_FlipY = false;
    };

} // Maize