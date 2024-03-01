#pragma once

namespace Maize {

    struct Texture
    {
    public:
        Texture() = default;
		explicit Texture(sf::Texture&& texture) : m_Texture(texture) { }
        explicit Texture(const std::string& filePath)
        {
            m_Texture.loadFromFile(filePath);
        }

        bool IsRegionTransparent(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
        {
            sf::Image image = m_Texture.copyToImage();

			// bounds check
			if (x + width > image.getSize().x || y + height > image.getSize().y) return false;

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

		sf::Vector2u GetSize() const { return m_Texture.getSize(); }

    private:
        friend class Sprite;

        const sf::Texture& GetTexture() const { return m_Texture; }

        sf::Texture m_Texture;
    };

} // Maize