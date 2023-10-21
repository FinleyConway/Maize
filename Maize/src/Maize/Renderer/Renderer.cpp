#include "mpch.h"
#include "Maize/Renderer/Renderer.h"
#include "Maize/Core/Window.h"
#include "Maize/Core/Application.h"

namespace Maize {

    Renderer::Renderer(Window& window) : m_Window(window)
    {
        CreateTextures();
    }

    sf::RenderTexture* Renderer::GetCurrentTexture()
    {
        return &m_Textures[m_CurrentTextureIndex];
    }

    sf::RenderTexture* Renderer::GetFinishedTexture()
    {
        uint32_t finishedTextureIndex = (m_CurrentTextureIndex + 1) % 2;
        return &m_Textures[finishedTextureIndex];
    }

    void Renderer::CreateTextures()
    {
        for (auto& m_Texture : m_Textures)
        {
            m_Texture.create(m_Window.GetWidth(), m_Window.GetHeight());
            m_Texture.clear();
            m_Texture.display();
        }
    }

    void Renderer::SwapTextures()
    {
        m_CurrentTextureIndex = (m_CurrentTextureIndex + 1) % 2; // toggle between 0 and 1
    }

    void Renderer::BeginSceneDrawing()
    {
        m_Textures[m_CurrentTextureIndex].clear();
    }

    void Renderer::BeginDrawing()
    {
        m_IsDrawing = true;
        m_DrawCalls = 0;
    }

    void Renderer::Draw(const sf::Shape& shape, sf::RenderTarget* renderTarget)
    {
        if (renderTarget == nullptr)
        {
            if (!m_Window.GetViewSpace().intersects(shape.getGlobalBounds()))
            {
                return;
            }
        }

        Draw(static_cast<const sf::Drawable&>(shape), sf::RenderStates::Default, renderTarget);
    }

    void Renderer::Draw(const sf::Sprite& sprite, sf::RenderTarget* renderTarget)
    {
        if (renderTarget == nullptr)
        {
            if (!m_Window.GetViewSpace().intersects(sprite.getGlobalBounds()))
            {
                return;
            }
        }

        Draw(static_cast<const sf::Drawable&>(sprite), sf::RenderStates::Default,  renderTarget);
    }

    void Renderer::Draw(const sf::Drawable& drawable, const sf::RenderStates& state, sf::RenderTarget* renderTarget)
    {
        // look more into this section in the future
        if (renderTarget == nullptr)
        {
            renderTarget = &m_Textures[m_CurrentTextureIndex];
        }

        renderTarget->draw(drawable, state);
        m_DrawCalls++;
    }

    void Renderer::DrawBufferTexture()
    {
        auto& window = Application::Get().GetWindow().GetRenderWindow();

        m_BufferSprite.setTexture(m_Textures[m_CurrentTextureIndex].getTexture());
        Draw(m_BufferSprite, &window);
    }

    void Renderer::EndDrawing() { m_IsDrawing = false; }

    void Renderer::EndSceneDrawing()
    {
        SwapTextures();
    }

    bool Renderer::IsDrawing() const { return m_IsDrawing; }

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, sf::Vector2f size, sf::Vector2f texCoord)
	{
		std::array<sf::Vertex, 4> quad;

		quad[0].position = sf::Vector2f((position.x + 0) * size.x, (position.y + 0) * size.y);
		quad[2].position = sf::Vector2f((position.x + 1) * size.x, (position.y + 1) * size.y);
		quad[1].position = sf::Vector2f((position.x + 1) * size.x, (position.y + 0) * size.y);
		quad[3].position = sf::Vector2f((position.x + 0) * size.x, (position.y + 1) * size.y);

		quad[0].texCoords = sf::Vector2f((texCoord.x + 0) * size.x, (texCoord.y + 0) * size.y);
		quad[1].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, (texCoord.y + 0) * size.y);
		quad[2].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, (texCoord.y + 1) * size.y);
		quad[3].texCoords = sf::Vector2f((texCoord.x + 0) * size.x, (texCoord.y + 1) * size.y);

		return quad;
	}

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoord)
	{
		sf::Transform transform;
		std::array<sf::Vertex, 4> quad;

		transform.translate(position);
		transform.rotate(rotation);

		quad[0].position = transform.transformPoint(-size / 2.0f);
		quad[1].position = transform.transformPoint(sf::Vector2f(size.x / 2.0f, -size.y / 2.0f));
		quad[2].position = transform.transformPoint(size / 2.0f);
		quad[3].position = transform.transformPoint(sf::Vector2f(-size.x / 2.0f, size.y / 2.0f));

		quad[0].texCoords = sf::Vector2f(texCoord.x, texCoord.y);
		quad[1].texCoords = sf::Vector2f(texCoord.x + size.x, texCoord.y);
		quad[2].texCoords = sf::Vector2f(texCoord.x + size.x, texCoord.y + size.y);
		quad[3].texCoords = sf::Vector2f(texCoord.x, texCoord.y + size.y);

		return quad;
	}

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Color colour)
	{
		sf::Transform transform;
		std::array<sf::Vertex, 4> quad;

		transform.translate(position);
		transform.rotate(rotation);

		quad[0].position = transform.transformPoint(-size / 2.0f);
		quad[1].position = transform.transformPoint(sf::Vector2f(size.x / 2.0f, -size.y / 2.0f));
		quad[2].position = transform.transformPoint(size / 2.0f);
		quad[3].position = transform.transformPoint(sf::Vector2f(-size.x / 2.0f, size.y / 2.0f));

		for (uint8_t i = 0; i < 4; i++)
		{
			quad[i].color = colour;
		}

		return quad;
	}

} // Maize