#pragma once

namespace Maize {

    struct Sprite
    {
    public:
        Sprite() = default;
        Sprite(const sf::IntRect rect, const Texture* texture, sf::Vector2f pivot = sf::Vector2f(0, 0))
        {
            m_Sprite.setOrigin(pivot);

            if (texture != nullptr)
            {
                m_Sprite.setTextureRect(rect);
                m_Sprite.setTexture(*texture);
            }
        }

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

        static bool IsSpriteInRect(const sf::Sprite& sprite, const sf::FloatRect& rect)
        {
            sf::FloatRect spriteBounds = sprite.getGlobalBounds();

            if (spriteBounds.intersects(rect))
            {
                return true;
            }

            return false;
        }

        sf::Sprite& GetSprite() { return m_Sprite; }

    private:
        sf::Sprite m_Sprite;
        bool m_FlipX = false;
        bool m_FlipY = false;
    };

} // Maize