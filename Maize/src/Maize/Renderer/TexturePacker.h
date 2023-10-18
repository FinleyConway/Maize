#pragma once

namespace Maize {

    struct TextureInfo
    {
        sf::Texture texture;
        int32_t id = -1;
    };

    struct PackTextureResults
    {
        sf::Texture packedTexture;
        std::unordered_map<int32_t, sf::IntRect> idToTextureRect;
    };

    class TexturePacker
    {
    public:
		static sf::Texture Pack(std::vector<sf::Texture>& textures, sf::Vector2u atlasSize);
        static PackTextureResults Pack(std::vector<TextureInfo>& textures, sf::Vector2u atlasSize);

    private:
		static void DetermineRectangles(const sf::IntRect& occupiedRect, sf::IntRect& smallestRect, sf::IntRect& largestRect, sf::Vector2u atlasSize);
		static sf::IntRect CreateRectUnderneath(const sf::IntRect& rect, sf::Vector2u atlasSize);
        static sf::IntRect CreateRectToRightOf(const sf::IntRect& rect, sf::Vector2u atlasSize);

		static bool TryPlaceInRect(const sf::Texture& texture, sf::Image& packedImage, std::vector<sf::IntRect>& occupiedRectangles, const sf::IntRect& rect);
		static void PlaceTextureInAtlas(const sf::Texture& texture, sf::Image& packedImage, std::vector<sf::IntRect>& occupiedRectangles, sf::Vector2i position);

        static bool IntersectsExistingRectangles(const sf::IntRect& rect, std::vector<sf::IntRect>& occupiedRectangles);
        static bool DoesRectFitInRect(sf::Vector2u textureSize, sf::Vector2i testRectSize);
		static bool CompareTextures(const sf::Texture& texture1, const sf::Texture& texture2);
		static bool CompareTexturesInfo(const TextureInfo& texture1, const TextureInfo& texture2);
	};

} // Maize