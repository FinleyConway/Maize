#include "Scene/SpriteSheetManager.h"

namespace Maize {

	SpriteSheetManager::SpriteSheetManager(Renderer& renderer) : m_Renderer(renderer)
	{
		// create a default sprite
		std::unordered_map<std::string, Sprite> spriteMap;
		spriteMap.try_emplace("Error");

		m_SpriteSheets["Error"] = spriteMap;
	}

	void SpriteSheetManager::AddSprites(const std::string& texturePath, const std::vector<Sprite>& sprites)
	{
		if (m_SpriteSheets.contains(texturePath))
		{
			std::cout << texturePath << " already exist for a sprite sheet!" << std::endl;
			return;
		}

		Texture texture = LoadTexture(texturePath);

		if (texture.IsValid())
		{
			m_Textures[texturePath] = std::move(texture);

			std::unordered_map<std::string, Sprite> spriteMap;

			for (const auto& sprite : sprites)
			{
				if (!spriteMap.contains(sprite.name))
				{
					spriteMap[sprite.name] = sprite;
				}
				else
				{
					std::cout << sprite.name << " is already in sprite sheet!" << std::endl;
				}
			}

			m_SpriteSheets[texturePath] = spriteMap;
		}
	}

	SpriteData SpriteSheetManager::GetSprite(const std::string& texturePath, const std::string& spriteName)
	{
		if (m_SpriteSheets.contains(texturePath))
		{
			auto& spriteSheet = m_SpriteSheets[texturePath];

			if (spriteSheet.contains(spriteName))
			{
				return { spriteSheet.at(spriteName), m_Textures[texturePath] };
			}
			else
			{
				std::cout << spriteName << " for sprite sheet cannot be found!" << std::endl;
				return { m_SpriteSheets["Error"].at("Error"), Texture() };
			}
		}
		else
		{
			std::cout << texturePath << " for sprite sheet cannot be found!" << std::endl;
			return { m_SpriteSheets["Error"].at("Error"), Texture() };
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

		return Texture(newTexture, Point(loadedSurface->w, loadedSurface->h));
	}

}