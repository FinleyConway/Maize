#include "mpch.h"
#include "Maize/Renderer/Renderer.h"
#include "Maize/Core/Application.h"

#include "Maize/Scene/Components.h"

namespace Maize {

	void Renderer::Initialize(sf::RenderWindow& window)
	{
		m_RenderWindow = &window;

		m_DefaultView.setCenter(0, 0);
		m_DefaultView.setViewport({ 0, 0, 1, 1 });
		m_DefaultView.setSize(static_cast<sf::Vector2f>(window.getSize()));
		m_RenderWindow->setView(m_DefaultView);
	}

	void Renderer::OnWindowResize(sf::Vector2f resize)
	{
		MZ_ASSERT_CORE(m_RenderWindow != nullptr, "Renderer is null!, please assign render window!");

		m_DefaultView.setSize(resize);
		m_RenderWindow->setView(m_DefaultView);
	}

	void Renderer::SetClearColour(sf::Color clearColour)
	{
		m_ClearColour = clearColour;
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

		m_Drawables.push_back({ &sprite, sprite.GetGlobalBounds(), spriteRenderer.sortingLayer, spriteRenderer.orderInLayer });
	}

	void Renderer::RemoveDrawable(const sf::Drawable* drawable)
	{
		if (drawable == nullptr)
		{
			MZ_LOG_CORE_WARN("Attempting to remove a null object!");
			return;
		}

		// finds the element
		auto it = std::find_if(m_Drawables.begin(), m_Drawables.end(),
			[&drawable](const RenderData& data) { return data.drawable == drawable; });

		// removes element if found
		if (it != m_Drawables.end())
		{
			m_Drawables.erase(it);
		}
	}

	void Renderer::UpdateDrawable(const Transform& transform, SpriteRenderer& spriteRenderer)
	{
		// finds the element
        auto& sprite = spriteRenderer.sprite;
		auto it = std::find_if(m_Drawables.begin(), m_Drawables.end(),
			[&sprite](const RenderData& d) { return d.drawable == &sprite; });

		// update element if found
		if (it != m_Drawables.end())
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

	void Renderer::BeginDrawing()
    {
		MZ_ASSERT_CORE(m_RenderWindow != nullptr, "Renderer is null!, please assign render window!");

		// mark as begin drawing
		m_RenderWindow->clear(m_ClearColour);

		m_IsDrawing = true;
		m_DrawCalls = 0;
    }

	void Renderer::DrawScene()
	{
		// TODO: Look into something like a quad tree to only naive through nearby objects!
		std::sort(m_Drawables.begin(), m_Drawables.end());

		// naive through entire scene
		for (const auto& draw : m_Drawables)
		{
			// only draw if inside the current viewport
			if (InsideViewport(draw))
			{
				DrawImmediately(draw.drawable);
			}
		}

		PhysicsEngine::DrawDebug(*m_RenderWindow);
	}

	void Renderer::DrawImmediately(const sf::Drawable* drawable)
	{
		MZ_ASSERT_CORE(m_RenderWindow != nullptr, "Renderer is null!, please assign render window!");

		if (drawable == nullptr)
		{
			MZ_LOG_CORE_WARN("Attempting to draw a null object!");
			return;
		}

		m_RenderWindow->draw(*drawable);

		m_DrawCalls++;
	}

	void Renderer::EndDrawing()
	{
		MZ_ASSERT_CORE(m_RenderWindow != nullptr, "Renderer is null!, please assign render window!");

		// mark as ended drawing
		m_RenderWindow->display();

		m_IsDrawing = false;
	}

	bool Renderer::IsDrawing() const
	{
		return m_IsDrawing;
	}

	size_t Renderer::GetDrawCall() const
	{
		return m_DrawCalls;
	}

	bool Renderer::InsideViewport(const RenderData& renderData)
	{
		const auto& view = m_RenderWindow->getView();

		const auto size = view.getSize();
		const auto halfSize = size / 2.0f;
		const auto viewSpace = sf::FloatRect(view.getCenter() - halfSize, size);

		if (viewSpace.intersects(renderData.bounds))
		{
			return true;
		}

		return false;
	}

} // Maize