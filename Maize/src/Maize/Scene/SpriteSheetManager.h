#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <format>
#include <iostream>
#include <unordered_map>

#include "Maize/Renderer/Renderer.h"
#include "Maize/Renderer/Texture.h"
#include "Maize/Renderer/Sprite.h"
#include "Maize/Math/Point.h"
#include "Maize/Math/Rect.h"

namespace Maize {

	class SpriteSheetManager
	{
	public:
		explicit SpriteSheetManager(Renderer& renderer);

        void AddSpritesFromSheet(Point spriteStartPosition, Point spriteEndPosition, Point size, PointF pivot, float ppu, const std::string& texturePath, const std::string& prefix);
		void AddSpriteFromSheet(Point position, Point size, PointF pivot, float ppu, const std::string& texturePath, const std::string& spriteName);

		const Sprite* GetSprite(const std::string& texturePath, const std::string& spriteName);

		void RemoveTexture(const std::string& texturePath);
		void RemoveSprite(const std::string& texturePath, const std::string& spriteName);

	private:
        std::unordered_map<std::string, Texture> m_Textures;
		std::unordered_map<std::string, std::unordered_map<std::string, Sprite>> m_SpriteSheets;
        Renderer& m_Renderer;

        Texture LoadTexture(const std::string& filePath) const;
		void AddSprites(Point spriteStartPosition, Point spriteEndPosition, Point size, PointF pivot, float ppu, const std::string& texturePath, const std::string& prefix);
	};

}