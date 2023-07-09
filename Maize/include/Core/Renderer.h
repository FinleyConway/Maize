#pragma once

#include <SDL.h>
#include <SDL_image.h>	
#include <memory>

#include "Window.h"
#include "Texture.h"
#include "Scene/Components.h"

namespace Maize {

	enum class RenderMode
	{
		NonPixelPerfect,
		PixelPerfect,
	};

	class Renderer
	{
	public:
		Renderer(const Window& window, int32_t index = -1, uint32_t flags = SDL_RENDERER_ACCELERATED);

		void SetIntegerScale(bool isIntegerScale);
		void SetLogicalSize(Vec2Int size);
		void SetScale(Vec2 scale);
		void SetViewport(Vec2Int position, Vec2Int size);
		void SetVSync(bool vsync);

		void SetTarget(const Texture* texture);

		bool GetIntegerScale() const;
		Vec2Int GetLogicalSize() const;
		Vec2 GetScale() const;
		SDL_Rect GetViewPort() const;

		void Clear();
		void RenderSprite(const Texture& texture, const SDL_Rect& dest, float angle, SDL_Point point, SDL_RendererFlip flip);
		void Present();

		operator SDL_Renderer*() const;

	private:
		std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_Renderer{ nullptr, &SDL_DestroyRenderer };
	};

}