#include "mpch.h"
#include "Maize/Renderer/TexturePacker.h"

namespace Maize {

	sf::Texture TexturePacker::Pack(std::vector<sf::Texture>& textures, sf::Vector2u atlasSize)
	{
		sf::Texture packedTexture;
		sf::Image packedImage;
		std::vector<sf::IntRect> occupiedRectangles;

		if (textures.empty()) return packedTexture;

		packedImage.create(atlasSize.x, atlasSize.y);

		// sort textures by largest to smallest before packing.
		std::sort(textures.begin(), textures.end(), CompareTextures);

		for (auto& texture : textures)
		{
			// check if there are any available slots in the atlas.
			if (occupiedRectangles.empty())
			{
				// place the texture in the top-left corner.
				PlaceTextureInAtlas(texture, packedImage, occupiedRectangles, sf::Vector2i(0, 0));
				continue;
			}

			// attempt to place the texture in available slots.
			for (const auto& inside : occupiedRectangles)
			{
				sf::IntRect smallestRect;
				sf::IntRect largestRect;

				DetermineRectangles(inside, smallestRect, largestRect, atlasSize);

				if (TryPlaceInRect(texture, packedImage, occupiedRectangles, smallestRect))
				{
					/*
					 * TODO: Add error handling here
					*/
					break;
				}
				else if (TryPlaceInRect(texture, packedImage, occupiedRectangles, largestRect))
				{
					/*
					 * TODO: Add error handling here
					*/
					break;
				}

				/*
				* TODO: Add error handling here
				*/
			}
		}

		packedTexture.loadFromImage(packedImage);
		return packedTexture;
	}

	void TexturePacker::DetermineRectangles(const sf::IntRect& occupiedRect, sf::IntRect& smallestRect, sf::IntRect& largestRect, sf::Vector2u atlasSize)
	{
		if (occupiedRect.width > occupiedRect.height)
		{
			smallestRect = CreateRectUnderneath(occupiedRect, atlasSize);
			largestRect = CreateRectToRightOf(occupiedRect, atlasSize);
		}
		else
		{
			smallestRect = CreateRectToRightOf(occupiedRect, atlasSize);
			largestRect = CreateRectUnderneath(occupiedRect, atlasSize);
		}
	}

	sf::IntRect TexturePacker::CreateRectUnderneath(const sf::IntRect& rect, sf::Vector2u atlasSize) const
    {
        return sf::IntRect(rect.left, rect.top + rect.height, rect.width, atlasSize.y - (rect.top + rect.height));
    }

    sf::IntRect TexturePacker::CreateRectToRightOf(const sf::IntRect& rect, sf::Vector2u atlasSize) const
    {
        return sf::IntRect(rect.left + rect.width, rect.top, atlasSize.x - (rect.left + rect.width), rect.height);
    }

	bool TexturePacker::TryPlaceInRect(const sf::Texture& texture, sf::Image& packedImage, std::vector<sf::IntRect>& occupiedRectangles, const sf::IntRect& rect)
	{
		if (DoesRectFitInRect(texture.getSize(), rect.getSize()) && !IntersectsExistingRectangles(rect, occupiedRectangles))
		{
			sf::Vector2i pos = rect.getPosition();

			packedImage.copy(texture.copyToImage(), pos.x, pos.y);
			occupiedRectangles.emplace_back(pos.x, pos.y, texture.getSize().x, texture.getSize().y);

			return true;
		}

		return false;
	}

	void TexturePacker::PlaceTextureInAtlas(const sf::Texture& texture, sf::Image& packedImage, std::vector<sf::IntRect>& occupiedRectangles, const sf::Vector2i& position)
	{
		packedImage.copy(texture.copyToImage(), position.x, position.y);
		occupiedRectangles.emplace_back(position.x, position.y, texture.getSize().x, texture.getSize().y);
	}

    bool TexturePacker::IntersectsExistingRectangles(const sf::IntRect& rect, std::vector<sf::IntRect>& occupiedRectangles) const
    {
        for (const auto& existingRect : occupiedRectangles)
        {
            if (rect.intersects(existingRect))
            {
                return true;
            }
        }

        return false;
    }

    bool TexturePacker::DoesRectFitInRect(sf::Vector2u textureSize, sf::Vector2i testRectSize) const
    {
        return textureSize.x <= testRectSize.x && textureSize.y <= testRectSize.y;
    }

	bool TexturePacker::CompareTextures(const sf::Texture& texture1, const sf::Texture& texture2)
	{
		uint32_t area1 = texture1.getSize().x * texture1.getSize().y;
		uint32_t area2 = texture2.getSize().x * texture2.getSize().y;

		return area1 > area2;
	}

} // Maize