#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <format>
#include <iostream>
#include <unordered_map>

#include "Core/Renderer.h"
#include "Core/Texture.h"
#include "Core/Point.h"
#include "Core/Sprite.h"
#include "Core/Rect.h"

namespace Maize {

	class SpriteSheetManager
	{
	public:
		explicit SpriteSheetManager(Renderer& renderer);

        void AddSpritesFromSheet(Point spriteStartPosition, Point spriteEndPosition, Point size, Point pivot, float ppu, const std::string& texturePath, const std::string& prefix);
		void AddSpriteFromSheet(Point position, Point size, Point pivot, float ppu, const std::string& texturePath, const std::string& spriteName);

		const Sprite* GetSprite(const std::string& texturePath, const std::string& spriteName);

		void RemoveTexture(const std::string& texturePath);
		void RemoveSprite(const std::string& texturePath, const std::string& spriteName);

	private:
        std::unordered_map<std::string, Texture> m_Textures;
		std::unordered_map<std::string, std::unordered_map<std::string, Sprite>> m_SpriteSheets;
        Renderer& m_Renderer;

        Texture LoadTexture(const std::string& filePath) const;
		void AddSprites(Point spriteStartPosition, Point spriteEndPosition, Point size, Point pivot, float ppu, const std::string& texturePath, const std::string& prefix);
	};

}