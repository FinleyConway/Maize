#pragma once

// https://en.wikipedia.org/wiki/Best-fit_bin_packing

namespace Maize {

    class TexturePacking
    {
    public:
        explicit TexturePacking(sf::Vector2u atlasSize);

        void Pack(std::vector<sf::IntRect> rects);
        const sf::Texture& GetTexture();

    private:
        bool IntersectsExistingRectangles(const sf::IntRect& rect) const;
        bool DoesRectFitInAtlas(const sf::IntRect& rect) const;
        bool DoesRectFitInRect(const sf::IntRect& rect, const sf::IntRect& testRect) const;
        static bool CompareRects(const sf::IntRect& rect1, const sf::IntRect& rect2);

        std::vector<sf::IntRect> CreatePossibleRects();
        void FindSuitableRect(const sf::IntRect& rect, sf::Color colour);

    private:
        std::vector<sf::IntRect> m_InsideAtlas;
        sf::Vector2u m_AtlasSize;
        sf::RenderTexture m_Atlas;
    };

} // Maize