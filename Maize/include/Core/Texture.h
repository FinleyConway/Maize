#pragma once

#include <SDL.h>
#include <memory>

#include "Math/Vec2UInt.h"
#include "Scene/Asset.h" 

namespace Maize {

	struct Texture : public Asset
	{
	public:
		Texture() = default;
		Texture(SDL_Texture* texture, Vec2Int size) : m_Texture(texture, &SDL_DestroyTexture), m_Size(size)
		{
		}

		void SetAlpha(uint8_t alpha) const
		{
			SDL_SetTextureAlphaMod(m_Texture.get(), alpha);
		}

		void SetColour(uint8_t r, uint8_t g, uint8_t b) const
		{
			SDL_SetTextureColorMod(m_Texture.get(), r, g, b);
		}

		Vec2Int Size() const { return m_Size; }

		operator SDL_Texture*() const { return m_Texture.get(); }

	private:
		friend class AssetManager;

		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_Texture = { nullptr, &SDL_DestroyTexture };
		Vec2Int m_Size = { 0, 0 };

		bool IsValid() const
		{
			return m_Texture != nullptr;
		}
	};

}