#pragma once

// https://en.wikipedia.org/wiki/Best-fit_bin_packing

#include "Maize/Utils/Random.h"

namespace Maize {

    class TexturePacking
    {
    public:
        explicit TexturePacking(sf::Vector2u atlasSize);

        void Pack(std::vector<sf::IntRect>& rects);
        const sf::Texture& GetTexture();

    private:
        sf::IntRect CreateRectUnderneath(const sf::IntRect& rect) const;
        sf::IntRect CreateRectToRightOf(const sf::IntRect& rect) const;
        bool IntersectsExistingRectangles(const sf::IntRect& rect) const;
        bool DoesRectFitInRect(const sf::IntRect& rect, const sf::IntRect& testRect) const;
        static bool CompareRects(const sf::IntRect& rect1, const sf::IntRect& rect2);

    private:
        std::vector<sf::IntRect> m_InsideAtlas;
        sf::Vector2u m_AtlasSize;
        sf::RenderTexture m_Atlas;
    };

} // Maize