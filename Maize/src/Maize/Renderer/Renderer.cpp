#include "mpch.h"
#include "Maize/Renderer/Renderer.h"
#include "Maize/Core/Application.h"

#include "Maize/Renderer/Sprite.h"

#include "Maize/Scene/Components.h"

namespace Maize {

	void Renderer::Initialize(sf::RenderWindow& window)
	{
		s_RenderWindow = &window;

		s_DefaultView.setCenter(0, 0);
		s_DefaultView.setViewport({ 0, 0, 1, 1 });
		s_DefaultView.setSize(static_cast<sf::Vector2f>(window.getSize()));
		s_RenderWindow->setView(s_DefaultView);
	}

	void Renderer::OnWindowResize(sf::Vector2f resize)
	{
		s_DefaultView.setSize(resize);
		s_RenderWindow->setView(s_DefaultView);
	}

	void Renderer::InsertDrawable(const Transform& transform, SpriteRenderer& spriteRenderer)
	{
        auto& sprite = spriteRenderer.sprite;
        const float scaling = m_PixelPerUnit / sprite.GetPixelPerUnit();

		sprite.setPosition(transform.position.x * m_PixelPerUnit, transform.position.y * m_PixelPerUnit * m_Flip);
		sprite.setRotation(transform.angle);
		sprite.setScale(transform.scale.x * scaling, transform.scale.y * scaling * m_Flip);

        if (spriteRenderer.flipX)
        {
            sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
        }

        if (spriteRenderer.flipY)
        {
            sprite.setScale(sprite.getScale().x, -sprite.getScale().y);
        }

		s_Drawables.push_back({ &sprite, sprite.GetGlobalBounds(), spriteRenderer.sortingLayer, spriteRenderer.orderInLayer });
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

	void Renderer::UpdateDrawable(const Transform& transform, SpriteRenderer& spriteRenderer)
	{
		// finds the element
        auto& sprite = spriteRenderer.sprite;
		auto it = std::find_if(s_Drawables.begin(), s_Drawables.end(),
			[&sprite](const RenderData& d) { return d.drawable == &sprite; });

		// update element if found
		if (it != s_Drawables.end())
		{
			const float scaling = m_PixelPerUnit / sprite.GetPixelPerUnit();

			sprite.setPosition(transform.position.x * m_PixelPerUnit, transform.position.y * m_PixelPerUnit * m_Flip);
			sprite.setRotation(transform.angle);
			sprite.setScale(transform.scale.x * scaling, transform.scale.y * scaling * m_Flip);

            if (spriteRenderer.flipX)
            {
                sprite.setScale(-sprite.getScale().x, sprite.getScale().y);
            }

            if (spriteRenderer.flipY)
            {
                sprite.setScale(sprite.getScale().x, -sprite.getScale().y);
            }

			it->bounds = sprite.GetGlobalBounds();
			it->sortingLayer = spriteRenderer.sortingLayer;
			it->orderInLayer = spriteRenderer.orderInLayer;
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

		auto size = view.getSize();
		auto halfSize = size / 2.0f;
		auto viewSpace = sf::FloatRect(view.getCenter() - halfSize, size);

		if (viewSpace.intersects(renderData.bounds))
		{
			return true;
		}

		return false;
	}

} // Maize