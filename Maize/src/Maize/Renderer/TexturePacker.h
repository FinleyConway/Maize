#pragma once

namespace Maize {

    class TexturePacker
    {
    public:
		sf::Texture Pack(std::vector<sf::Texture>& textures, sf::Vector2u atlasSize);

    private:
		void DetermineRectangles(const sf::IntRect& occupiedRect, sf::IntRect& smallestRect, sf::IntRect& largestRect, sf::Vector2u atlasSize);
		sf::IntRect CreateRectUnderneath(const sf::IntRect& rect, sf::Vector2u atlasSize) const;
        sf::IntRect CreateRectToRightOf(const sf::IntRect& rect, sf::Vector2u atlasSize) const;

		bool TryPlaceInRect(const sf::Texture& texture, sf::Image& packedImage, std::vector<sf::IntRect>& occupiedRectangles, const sf::IntRect& rect);
		void PlaceTextureInAtlas(const sf::Texture& texture, sf::Image& packedImage, std::vector<sf::IntRect>& occupiedRectangles, const sf::Vector2i& position);

        bool IntersectsExistingRectangles(const sf::IntRect& rect, std::vector<sf::IntRect>& occupiedRectangles) const;
        bool DoesRectFitInRect(sf::Vector2u textureSize, sf::Vector2i testRectSize) const;
		static bool CompareTextures(const sf::Texture& texture1, const sf::Texture& texture2);
	};

} // Maize