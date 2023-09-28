#include "mpch.h"
#include "Maize/Renderer/TexturePacking.h"

#include "Maize/Utils/Random.h"

namespace Maize {

    TexturePacking::TexturePacking(sf::Vector2u atlasSize) : m_AtlasSize(atlasSize)
    {
        m_Atlas.create(atlasSize.x, atlasSize.y);
        m_Atlas.clear();
    }

    void TexturePacking::Pack(std::vector<sf::IntRect> &rects)
    {
        auto copy = rects;

        std::sort(copy.begin(), copy.end(), CompareRects);

        for (const auto& rect : copy)
        {
            if (m_InsideAtlas.empty())
            {
                int32_t r = Random::Range(0, 255);
                int32_t g = Random::Range(0, 255);
                int32_t b = Random::Range(0, 255);

                sf::RectangleShape shape((sf::Vector2f)rect.getSize());
                shape.setPosition((sf::Vector2f(0, 0)));
                shape.setFillColor(sf::Color(r, g, b));
                m_Atlas.draw(shape);

                m_InsideAtlas.push_back(rect);

                continue;
            }

            for (const auto& inside : m_InsideAtlas)
            {
                sf::IntRect smallestRect;
                sf::IntRect largestRect;

                if (inside.width > inside.height)
                {
                    // create a rect underneath the existing rect
                    smallestRect = CreateRectUnderneath(inside);
                    largestRect = CreateRectToRightOf(inside);
                }
                else
                {
                    // create a rect on the right side of the existing rect
                    smallestRect = CreateRectToRightOf(inside);
                    largestRect = CreateRectUnderneath(inside);
                }

                if (DoesRectFitInRect(rect, smallestRect) && !IntersectsExistingRectangles(smallestRect))
                {
                    int32_t r = Random::Range(0, 255);
                    int32_t g = Random::Range(0, 255);
                    int32_t b = Random::Range(0, 255);

                    sf::RectangleShape shape(sf::Vector2f(rect.width, rect.height));
                    shape.setPosition((sf::Vector2f)smallestRect.getPosition());
                    shape.setFillColor(sf::Color(r, g, b));

                    m_Atlas.draw(shape);
                    m_InsideAtlas.emplace_back(smallestRect.getPosition().x, smallestRect.getPosition().y, rect.width, rect.height);

                    break;
                }
                else if (DoesRectFitInRect(rect, largestRect) && !IntersectsExistingRectangles(largestRect))
                {
                    int32_t r = Random::Range(0, 255);
                    int32_t g = Random::Range(0, 255);
                    int32_t b = Random::Range(0, 255);

                    sf::RectangleShape shape(sf::Vector2f(rect.width, rect.height));
                    shape.setPosition((sf::Vector2f)largestRect.getPosition());
                    shape.setFillColor(sf::Color(r, g, b));

                    m_Atlas.draw(shape);
                    m_InsideAtlas.emplace_back(largestRect.getPosition().x, largestRect.getPosition().y, rect.width, rect.height);

                    break;
                }
            }
        }
    }

    const sf::Texture &TexturePacking::GetTexture()
    {
        m_Atlas.display();
        return m_Atlas.getTexture();
    }

    sf::IntRect TexturePacking::CreateRectUnderneath(const sf::IntRect &rect) const
    {
        return sf::IntRect(rect.left, rect.top + rect.height, rect.width, m_AtlasSize.y - (rect.top + rect.height));
    }

    sf::IntRect TexturePacking::CreateRectToRightOf(const sf::IntRect &rect) const
    {
        return sf::IntRect(rect.left + rect.width, rect.top, m_AtlasSize.x - (rect.left + rect.width), rect.height);
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

    bool TexturePacking::DoesRectFitInRect(const sf::IntRect& rect, const sf::IntRect& testRect) const
    {
        return rect.width <= testRect.width && rect.height <= testRect.height;
    }

    bool TexturePacking::CompareRects(const sf::IntRect &rect1, const sf::IntRect &rect2)
    {
        int32_t area1 = rect1.width * rect1.height;
        int32_t area2 = rect2.width * rect2.height;

        return area1 > area2;
    }

} // Maize