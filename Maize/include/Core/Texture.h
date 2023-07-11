#pragma once

#include <SDL.h>
#include <memory>

#include "Point.h"

namespace Maize {

	struct Texture
	{
	public:
		Texture() = default;
		Texture(SDL_Texture* texture, Point size) : m_Texture(texture, &SDL_DestroyTexture), m_Size(size)
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

		Point Size() const { return m_Size; }

		operator SDL_Texture*() const { return m_Texture.get(); }

	private:
		friend class SpriteSheetManager;

		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_Texture = { nullptr, &SDL_DestroyTexture };
		Point m_Size;

		bool IsValid() const
		{
			return m_Texture != nullptr;
		}
	};

}