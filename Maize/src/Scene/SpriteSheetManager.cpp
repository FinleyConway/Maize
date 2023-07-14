#include "Scene/SpriteSheetManager.h"

namespace Maize {

	SpriteSheetManager::SpriteSheetManager(Renderer& renderer) : m_Renderer(renderer) { }

	void SpriteSheetManager::AddSpritesFromSheet(Point spriteStartPosition, Point spriteEndPosition, Point size, Point pivot, uint32_t ppu, const std::string& texturePath, const std::string& prefix)
	{
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

	void SpriteSheetManager::AddSpriteFromSheet(Point position, Point size, Point pivot, uint32_t ppu, const std::string& texturePath, const std::string& spriteName)
	{
		if (m_SpriteSheets.contains(texturePath))
		{
			std::unordered_map<std::string, Sprite>& spriteMap = m_SpriteSheets[texturePath];
			SDL_Rect spriteRect = { position.x, position.y, size.x, size.y };

			spriteMap.try_emplace(spriteName, spriteName, spriteRect, pivot, ppu, m_Textures[texturePath]);
		}
		else
		{
			Texture texture = LoadTexture(texturePath);

			if (texture.IsValid())
			{
				m_Textures[texturePath] = std::move(texture);

				std::unordered_map<std::string, Sprite> spriteMap;
				SDL_Rect spriteRect = { position.x, position.y, size.x, size.y };

				spriteMap.try_emplace(spriteName, spriteName, spriteRect, pivot, ppu, m_Textures[texturePath]);
				m_SpriteSheets.try_emplace(texturePath, spriteMap);
			}
		}
	}

	const Sprite* SpriteSheetManager::GetSprite(const std::string& texturePath, const std::string& spriteName)
	{
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

	void SpriteSheetManager::AddSprites(Point spriteStartPosition, Point spriteEndPosition, Point size, Point pivot, uint32_t ppu, const std::string& texturePath, const std::string& prefix)
	{
		std::unordered_map<std::string, Sprite> spriteMap;
		uint32_t index = 0;

		for (int32_t x = spriteStartPosition.x; x <= spriteEndPosition.x; x++)
		{
			for (int32_t y = spriteStartPosition.y; y <= spriteEndPosition.y; y++)
			{
				std::string spriteName = std::format("{}{}", prefix, index);
				SDL_Rect spriteRect = { x * size.x, y * size.y, size.x, size.y };

				spriteMap.try_emplace(spriteName, spriteName, spriteRect, pivot, ppu, m_Textures[texturePath]);
				index++;
			}
		}

		m_SpriteSheets.try_emplace(texturePath, spriteMap);
	}

}