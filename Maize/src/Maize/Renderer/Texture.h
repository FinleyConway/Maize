#pragma once

namespace Maize {

    struct Texture
    {
    public:
        Texture() = default;
		explicit Texture(sf::Texture&& texture) : m_Texture(texture) { }

		static std::shared_ptr<Texture> Create(const std::string& filePath)
		{
			auto texture = std::make_shared<Texture>();

			if (!texture->m_Texture.loadFromFile(filePath))
			{
				return nullptr;
			}
			else
			{
				return std::move(texture);
			}
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
		const sf::Texture& GetTexture() const { return m_Texture; }

    private:
        sf::Texture m_Texture;
    };

} // Maize