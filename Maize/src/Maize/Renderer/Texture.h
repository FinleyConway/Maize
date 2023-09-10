#pragma once

#include <SFML/Graphics/Texture.hpp>
#include <cstdint>
#include <memory>

namespace Maize {

    struct Texture
    {
    public:
        Texture() = default;

        static std::shared_ptr<Texture> Create(const std::string& filePath)
        {
            auto texture = std::make_shared<Texture>();

            if (!texture->m_Texture.loadFromFile(filePath))
            {
                texture->m_IsValid = false;
            }
            else
            {
                texture->m_IsValid = true;
            }

            return texture;
        }

        bool IsRegionTransparent(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            sf::Image image = m_Texture.copyToImage();

            // loop through specific section of the texture to check its pixel information
            for (uint32_t i = x; i < x + width; i++)
            {
                for (uint32_t j = y; j < y + height; j++)
                {
                    sf::Color pixelColour = image.getPixel(i, j);

                    if (pixelColour.a != 0)
                    {
                        // pixels at region arent transparent
                        return false;
                    }
                }
            }

            // all pixels in region are transparent
            return true;
        }

        bool IsValid() const { return m_IsValid; }
        uint32_t GetWidth() const { return m_Texture.getSize().x; }
        uint32_t GetHeight() const { return m_Texture.getSize().y; }

        operator sf::Texture& () { return m_Texture; }
        operator const sf::Texture& () const { return m_Texture; }

    private:
        sf::Texture m_Texture;
        bool m_IsValid = false;
    };

} // Maize