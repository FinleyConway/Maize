#include "mpch.h"
#include "Maize/Renderer/TexturePacking.h"

#include "Maize/Utils/Random.h"

namespace Maize {

    TexturePacking::TexturePacking(sf::Vector2u atlasSize) : m_AtlasSize(atlasSize)
    {
        m_Atlas.create(atlasSize.x, atlasSize.y);
        m_Atlas.clear();
    }

    void TexturePacking::Pack(std::vector<sf::IntRect> rects)
    {
        std::sort(rects.begin(), rects.end(), CompareRects);

        for (const auto& rect : rects)
        {
            int32_t r = Random::Range(0, 255);
            int32_t g = Random::Range(0, 255);
            int32_t b = Random::Range(0, 255);

            FindSuitableRect(rect, sf::Color(r, g, b, 255));
        }
    }

    const sf::Texture &TexturePacking::GetTexture()
    {
        m_Atlas.display();
        return m_Atlas.getTexture();
    }

    bool TexturePacking::IntersectsExistingRectangles(const sf::IntRect &rect) const
    {
        for (const auto& existingRect : m_InsideAtlas)
        {
            if (rect.intersects(existingRect))
            {
                return true;
            }
        }

        return false;
    }

    bool TexturePacking::DoesRectFitInAtlas(const sf::IntRect &rect) const
    {
        return rect.height < m_AtlasSize.y || rect.width < m_AtlasSize.x;
    }

    bool TexturePacking::DoesRectFitInRect(const sf::IntRect &rect, const sf::IntRect &testRect) const
    {
        bool isSmallerWidth = rect.width <= testRect.width;
        bool isSmallerHeight = rect.height <= testRect.height;

        return isSmallerWidth && isSmallerHeight;
    }

    bool TexturePacking::CompareRects(const sf::IntRect &rect1, const sf::IntRect &rect2)
    {
        int32_t area1 = rect1.width * rect1.height;
        int32_t area2 = rect2.width * rect2.height;

        return area1 > area2;
    }

    std::vector<sf::IntRect> TexturePacking::CreatePossibleRects()
    {
        std::vector<sf::IntRect> newPossibleRects;

        if (m_InsideAtlas.empty()) return newPossibleRects;

        for (const auto& existingRect : m_InsideAtlas)
        {
            sf::IntRect rightRect(existingRect.left + existingRect.width, existingRect.top,
                                  m_AtlasSize.x - (existingRect.left + existingRect.width), existingRect.height);

            sf::IntRect bottomRect(existingRect.left, existingRect.top + existingRect.height,
                                   existingRect.width, m_AtlasSize.y - (existingRect.top + existingRect.height));

            if (rightRect.width > 0 && rightRect.height > 0 &&
                !IntersectsExistingRectangles(rightRect) &&
                DoesRectFitInAtlas(rightRect))
            {
                newPossibleRects.push_back(rightRect);
            }

            if (bottomRect.width > 0 && bottomRect.height > 0 &&
                !IntersectsExistingRectangles(bottomRect) &&
                DoesRectFitInAtlas(bottomRect))
            {
                newPossibleRects.push_back(bottomRect);
            }
        }

        return newPossibleRects;
    }

    void TexturePacking::FindSuitableRect(const sf::IntRect &rect, sf::Color colour)
    {
        auto possibleRects = CreatePossibleRects();

        if (possibleRects.empty())
        {
            m_InsideAtlas.emplace_back(0, 0, rect.width, rect.height);

            sf::RectangleShape coloredRect(sf::Vector2f(rect.width, rect.height));
            coloredRect.setPosition(sf::Vector2f(0, 0));
            coloredRect.setFillColor(colour);

            m_Atlas.draw(coloredRect);

            return;
        }

        for (const auto& pRect : possibleRects)
        {
            if (DoesRectFitInRect(rect, pRect))
            {
                auto position = pRect.getPosition();

                m_InsideAtlas.emplace_back(position.x, position.y, rect.width, rect.height);

                sf::RectangleShape coloredRect(sf::Vector2f(rect.width, rect.height));
                coloredRect.setPosition((sf::Vector2f)position);
                coloredRect.setFillColor(colour);

                m_Atlas.draw(coloredRect);

                break;
            }
        }
    }

} // Maize