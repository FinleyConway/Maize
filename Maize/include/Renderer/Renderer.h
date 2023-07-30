#pragma once

#include <SDL.h>
#include <memory>

#include "Core/Window.h"
#include "Texture.h"
#include "Math/Point.h"
#include "Math/Rect.h"

namespace Maize {

	class Renderer
	{
	public:
		Renderer(const Window& window, int32_t index = -1, uint32_t flags = SDL_RENDERER_ACCELERATED);

		void SetIntegerScale(bool isIntegerScale);
		void SetLogicalSize(Point size);
		void SetScale(PointF scale);
		void SetViewport(Point position, Point size);
		void SetVSync(bool vsync);
		void SetTarget(const Texture* texture);

		bool GetIntegerScale() const;
		Point GetLogicalSize() const;
		PointF GetScale() const;
		SDL_Rect GetViewPort() const;

		void Clear() const;
		static void RenderSprite(const Texture& texture, const Rect& spritePosition, const Rect& screenPosition, float angle, PointF point, SDL_RendererFlip flip);
		void Present() const;

		operator SDL_Renderer*() const;

	private:
		static std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)> m_Renderer;
	};

}