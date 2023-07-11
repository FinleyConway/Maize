#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <unordered_map>

#include "Core/Renderer.h"
#include "Core/Texture.h"
#include "Core/Point.h"

namespace Maize {

	struct Sprite
	{
        std::string name = "";
		SDL_Rect spritePosition = { 0, 0, 0 ,0 };
		Point pivot = { 0, 0 };

		Sprite() = default;
		Sprite(const std::string& name, const SDL_Rect& spritePosition, const Point& pivot)
			: name(name), spritePosition(spritePosition), pivot(pivot)
		{
		}
	};

	struct SpriteData
	{
		const Sprite& sprite;
		const Texture& texture;
	};

	class SpriteSheetManager
	{
	public:
		SpriteSheetManager(Renderer& renderer);

		void AddSprites(const std::string& texturePath, const std::vector<Sprite>& sprites);

		SpriteData GetSprite(const std::string& texturePath, const std::string& spriteName);

		void RemoveTexture(const std::string& texturePath);
		void RemoveSprite(const std::string& texturePath, const std::string& spriteName);

	private:
        std::unordered_map<std::string, Texture> m_Textures;
		std::unordered_map<std::string, std::unordered_map<std::string, Sprite>> m_SpriteSheets;
        Renderer& m_Renderer;

        Texture LoadTexture(const std::string& filePath) const;
	};

}