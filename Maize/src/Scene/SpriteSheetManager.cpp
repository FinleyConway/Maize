#include "Scene/SpriteSheetManager.h"

namespace Maize {

	SpriteSheetManager::SpriteSheetManager(Renderer& renderer) : m_Renderer(renderer) { }

	void SpriteSheetManager::AddSpritesFromSheet(Point spriteStartPosition, Point spriteEndPosition, Point size, Point pivot, float ppu, const std::string& texturePath, const std::string& prefix)
	{
		// load and add/pre-add texture with sprites corresponding to that texture
		if (m_SpriteSheets.contains(texturePath))
		{
			AddSprites(spriteStartPosition, spriteEndPosition, size, pivot, ppu, texturePath, prefix);
		}
		else
		{
			Texture texture = LoadTexture(texturePath);

			if (texture.IsValid())
			{
				m_Textures[texturePath] = std::move(texture);

				AddSprites(spriteStartPosition, spriteEndPosition, size, pivot, ppu, texturePath, prefix);
			}
		}
	}

	void SpriteSheetManager::AddSpriteFromSheet(Point position, Point size, Point pivot, float ppu, const std::string& texturePath, const std::string& spriteName)
	{
		// load and add/pre-add texture with sprites corresponding to that texture
		if (m_SpriteSheets.contains(texturePath))
		{
			Rect spriteRect = { position.x, position.y, size.x, size.y };

			m_SpriteSheets[texturePath].try_emplace(spriteName, spriteName, spriteRect, pivot, ppu, m_Textures[texturePath]);
		}
		else
		{
			Texture texture = LoadTexture(texturePath);

			if (texture.IsValid())
			{
				m_Textures[texturePath] = std::move(texture);

				Rect spriteRect = { position.x, position.y, size.x, size.y };

				m_SpriteSheets[texturePath].try_emplace(spriteName, spriteName, spriteRect, pivot, ppu, m_Textures[texturePath]);
			}
		}
	}

	const Sprite* SpriteSheetManager::GetSprite(const std::string& texturePath, const std::string& spriteName)
	{
		// get sprite from specific texture
		if (m_SpriteSheets.contains(texturePath))
		{
			auto& spriteSheet = m_SpriteSheets[texturePath];

			if (spriteSheet.contains(spriteName))
			{
				return &spriteSheet.at(spriteName);
			}
			else
			{
				std::cout << spriteName << " for sprite sheet cannot be found!" << std::endl;
				return nullptr;
			}
		}
		else
		{
			std::cout << texturePath << " for sprite sheet cannot be found!" << std::endl;
			return nullptr;
		}
	}

	void SpriteSheetManager::RemoveTexture(const std::string& texturePath)
	{
		// remove texture with sprites that corresponding with that texture
		if (m_SpriteSheets.contains(texturePath))
		{
			m_SpriteSheets.erase(texturePath);
			m_Textures.erase(texturePath);
		}
		else
		{
			std::cout << texturePath << " trying to remove this path from sprite sheet when it doesnt exist!" << std::endl;
		}
	}

	void SpriteSheetManager::RemoveSprite(const std::string& texturePath, const std::string& spriteName)
	{
		// remove sprite from corresponding texture
		if (m_SpriteSheets.contains(texturePath))
		{
			auto& spriteMap = m_SpriteSheets[texturePath];

			if (spriteMap.contains(spriteName))
			{
				spriteMap.erase(spriteName);
			}
			else
			{
				std::cout << spriteName << " trying to remove this sprite from sprite sheet when it doesnt exist!" << std::endl;
			}
		}
		else
		{
			std::cout << texturePath << " trying to remove " << spriteName << " from this path for when it doesnt exist!" << std::endl;
		}
	}

	Texture SpriteSheetManager::LoadTexture(const std::string& filePath) const
	{
		// load image
		SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());

		if (loadedSurface == nullptr)
		{
			std::cout << SDL_GetError() << std::endl;
			return Texture(); // return an invalid Texture object
		}

		// create texture
		SDL_Texture* newTexture = SDL_CreateTextureFromSurface(m_Renderer, loadedSurface);
		SDL_FreeSurface(loadedSurface); // free the loaded surface since we no longer need it

		if (newTexture == nullptr)
		{
			std::cout << SDL_GetError() << std::endl;
			return Texture(); // return an invalid Texture object
		}

		return Texture(newTexture);
	}

	void SpriteSheetManager::AddSprites(Point spriteStartPosition, Point spriteEndPosition, Point size, Point pivot, float ppu, const std::string& texturePath, const std::string& prefix)
	{
		std::unordered_map<std::string, Sprite> spriteMap;
		uint32_t index = 0;

		// loop through each sprite in a rectangle area of a sprite
		for (int32_t x = spriteStartPosition.x; x <= spriteEndPosition.x; x++)
		{
			for (int32_t y = spriteStartPosition.y; y <= spriteEndPosition.y; y++)
			{
				std::string spriteName = std::format("{}{}", prefix, index);
				Rect spriteRect = { x * size.x, y * size.y, size.x, size.y };

				m_SpriteSheets[texturePath].try_emplace(spriteName, spriteName, spriteRect, pivot, ppu, m_Textures[texturePath]);
				index++;
			}
		}
	}

}