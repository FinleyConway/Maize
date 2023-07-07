#include "Core/Renderer.h"

namespace Maize {

	Renderer::Renderer(const Window& window, int32_t index, uint32_t flags)
	{
		m_Renderer.reset(SDL_CreateRenderer(window, index, flags));
		if (m_Renderer == nullptr)
		{
			std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
		}
	}

	void Renderer::SetIntegerScale(bool isIntegerScale)
	{
		if (isIntegerScale)
			SDL_RenderSetIntegerScale(m_Renderer.get(), SDL_TRUE);
		else
			SDL_RenderSetIntegerScale(m_Renderer.get(), SDL_FALSE);
	}

	void Renderer::SetLogicalSize(Vec2Int size)
	{
		SDL_RenderSetLogicalSize(m_Renderer.get(), size.x, size.y);
	}

	void Renderer::SetScale(Vec2 scale)
	{
		SDL_RenderSetScale(m_Renderer.get(), scale.x, scale.y);
	}

	void Renderer::SetViewport(Vec2Int position, Vec2Int size)
	{
		SDL_Rect rect = { position.x, position.y, size.x, size.y };
		SDL_RenderSetViewport(m_Renderer.get(), &rect);
	}

	void Renderer::SetVSync(bool vSync)
	{
		SDL_RenderSetVSync(m_Renderer.get(), vSync);
	}

	void Renderer::SetTarget(const Texture* texture)
	{
		SDL_SetRenderTarget(m_Renderer.get(), *texture);
	}

	bool Renderer::GetIntegerScale() const
	{
		return SDL_RenderGetIntegerScale(m_Renderer.get());
	}

	Vec2Int Renderer::GetLogicalSize() const
	{
		int x, y;
		SDL_RenderGetLogicalSize(m_Renderer.get(), &x, &y);
		return Vec2Int(x, y);
	}

	Vec2 Renderer::GetScale() const
	{
		float x, y;
		SDL_RenderGetScale(m_Renderer.get(), &x, &y);
		return Vec2(x, y);
	}

	SDL_Rect Renderer::GetViewPort() const
	{
		SDL_Rect rect;
		SDL_RenderGetViewport(m_Renderer.get(), &rect);
		return rect;
	}

	void Renderer::Clear() 
	{ 
		SDL_RenderClear(m_Renderer.get());
	}

	void Renderer::RenderSprite(const Texture& texture, const SDL_Rect& dest, float angle, SDL_Point point)
	{
		SDL_RenderCopyEx(m_Renderer.get(), texture, nullptr, &dest, angle, &point, SDL_FLIP_NONE);
	}

	void Renderer::Present() 
	{ 
		SDL_RenderPresent(m_Renderer.get()); 
	}

	Renderer::operator SDL_Renderer*() const
	{
		return m_Renderer.get();
	}

}