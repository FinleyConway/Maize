#pragma once

#include <SDL.h>
#include <memory>

#include "Point.h"
#include "Colour.h"

namespace Maize {

	struct Texture
	{
	public:
		Texture() = default;
		explicit Texture(SDL_Texture* texture) : m_Texture(texture, &SDL_DestroyTexture)
		{
		}

		void SetAlpha(uint8_t alpha)
		{
			SDL_SetTextureAlphaMod(m_Texture.get(), alpha);
		}

		void SetAlpha(Colour colour)
		{
			SDL_SetTextureAlphaMod(m_Texture.get(), colour.a);
		}

		void SetColour(uint8_t r, uint8_t g, uint8_t b)
		{
			SDL_SetTextureColorMod(m_Texture.get(), r, g, b);
		}

		void SetColour(Colour colour)
		{
			SDL_SetTextureColorMod(m_Texture.get(), colour.r, colour.g, colour.b);
		}

		Point Size() const 
		{ 
			int32_t x, y;
			SDL_QueryTexture(m_Texture.get(), nullptr, nullptr, &x, &y);
			return Point(x, y);
		}

		bool IsValid() const
		{
			return m_Texture != nullptr;
		}

		operator SDL_Texture*() const { return m_Texture.get(); }

	private:
		std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> m_Texture = { nullptr, &SDL_DestroyTexture };
	};

}