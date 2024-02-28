#include "mpch.h"
#include "Maize/Renderer/Renderer.h"
#include "Maize/Core/Application.h"

namespace Maize {

	void Renderer::Initialize(sf::RenderWindow& window)
	{
		s_RenderWindow = &window;
	}

	void Renderer::InsertDrawable(const std::vector<RenderData>& renderData)
	{
		// pre alloc and insert elements into drawables
		s_Drawables.reserve(s_Drawables.size() + renderData.size());
		s_Drawables.insert(s_Drawables.end(), renderData.begin(), renderData.end());
	}

	void Renderer::InsertDrawable(const RenderData& renderData)
	{
		// add element to vector
		s_Drawables.emplace_back(renderData);
	}

	void Renderer::RemoveDrawable(const std::vector<sf::Drawable*>& drawables)
	{
		if (drawables.empty()) return;

		for (auto draw : drawables)
		{
			RemoveDrawable(draw);
		}
	}

	void Renderer::RemoveDrawable(const sf::Drawable* drawable)
	{
		// finds the element
		auto it = std::find_if(s_Drawables.begin(), s_Drawables.end(),
			[&drawable](const RenderData& data) { return data.drawable == drawable; });

		// removes element if found
		if (it != s_Drawables.end())
		{
			s_Drawables.erase(it);
		}
	}

	void Renderer::UpdateDrawable(const std::vector<RenderData>& renderData)
	{
		for (const auto& data : renderData)
		{
			// finds the element
			auto it = std::find_if(s_Drawables.begin(), s_Drawables.end(),
				[drawable = data.drawable](const RenderData& d) { return d.drawable == drawable; });

			// update element if found
			if (it != s_Drawables.end())
			{
				it->bounds = data.bounds;
				it->sortingLayer = data.orderInLayer;
				it->orderInLayer = data.orderInLayer;
			}
		}
	}

	void Renderer::UpdateDrawable(const RenderData& renderData)
	{
		// finds the element
		auto it = std::find_if(s_Drawables.begin(), s_Drawables.end(),
			[drawable = renderData.drawable](const RenderData& d) { return d.drawable == drawable; });

		// update element if found
		if (it != s_Drawables.end())
		{
			it->bounds = renderData.bounds;
			it->sortingLayer = renderData.orderInLayer;
			it->orderInLayer = renderData.orderInLayer;
		}
	}

	void Renderer::BeginDrawing(sf::Color clearColour)
    {
		if (s_RenderWindow == nullptr)
		{
			std::cerr << "Renderer is null!, please assign render window" << std::endl;
			return;
		}

		// mark as begin drawing
		s_RenderWindow->clear(clearColour);

		s_IsDrawing = true;
		s_DrawCalls = 0;
    }

	void Renderer::DrawScene()
	{
		// TODO: Look into something like a quad tree to only naive through nearby objects!
		std::sort(s_Drawables.begin(), s_Drawables.end());

		// naive through entire scene
		for (auto draw : s_Drawables)
		{
			// only draw if inside the current viewport
			if (InsideViewport(draw))
			{
				DrawImmediately(draw.drawable);
			}
		}
	}

	void Renderer::DrawImmediately(const std::vector<sf::Drawable*>& drawable)
	{
		for (auto draw : drawable)
		{
			DrawImmediately(draw);
		}
	}

	void Renderer::DrawImmediately(const sf::Drawable* drawable)
	{
		if (s_RenderWindow == nullptr)
		{
			std::cerr << "Renderer is null!, please assign render window" << std::endl;
			return;
		}

		if (drawable == nullptr)
		{
			std::cerr << "Attempting to draw a null object!" << std::endl;
			return;
		}

		s_RenderWindow->draw(*drawable);

		s_DrawCalls++;
	}

	void Renderer::EndDrawing()
	{
		if (s_RenderWindow == nullptr)
		{
			std::cerr << "Renderer is null!, please assign render window" << std::endl;
			return;
		}

		// mark as ended drawing
		s_RenderWindow->display();

		s_IsDrawing = false;
	}

	bool Renderer::InsideViewport(const RenderData& renderData)
	{
		auto view = s_RenderWindow->getView();
		auto viewport = view.getViewport();
		auto size = view.getSize();

		auto viewportBounds = sf::FloatRect(viewport.left * size.x, viewport.top * size.y, viewport.width * size.x, viewport.height * size.y);

		if (renderData.bounds.intersects(viewportBounds))
		{
			return true;
		}

		return false;
	}

} // Maize