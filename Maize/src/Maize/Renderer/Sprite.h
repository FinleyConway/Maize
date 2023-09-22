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

        void SetPosition(sf::Vector2f position) { m_Sprite.setPosition(position); }
        sf::Vector2f GetPosition() const { return m_Sprite.getPosition(); }

        void SetColour(sf::Color colour) { m_Sprite.setColor(colour); }
        sf::Color GetColour() const { return m_Sprite.getColor(); }

        const sf::Texture* GetTexture() const { return m_Sprite.getTexture(); }
        const sf::IntRect& GetTextureRect() const { return m_Sprite.getTextureRect(); }

        void SetPivot(sf::Vector2f pivot) { m_Sprite.setOrigin(pivot); }
        sf::Vector2f GetPivot() const { return m_Sprite.getOrigin(); }

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
        sf::Sprite m_Sprite;
        bool m_FlipX = false;
        bool m_FlipY = false;
    };

} // Maize