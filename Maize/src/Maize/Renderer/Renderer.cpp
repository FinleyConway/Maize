#include "mpch.h"
#include "Maize/Renderer/Renderer.h"
#include "Maize/Core/Window.h"
#include "Maize/Core/Application.h"

namespace Maize {

	void Renderer::Initialize(sf::Vector2u resize)
	{
		for (auto& texture : s_Textures)
		{
			texture = new sf::RenderTexture();
		}

		OnWindowResize(resize);
	}

	void Renderer::Shutdown()
	{
		for (auto& texture : s_Textures)
		{
			delete texture;
		}
	}

	void Renderer::OnWindowResize(sf::Vector2u resize)
	{
		CreateTextures(resize);

		auto view = GetCurrentTexture()->getView();
		view.setSize((sf::Vector2f)resize);
		GetCurrentTexture()->setView(view);
	}

    sf::RenderTexture* Renderer::GetCurrentTexture()
    {
        return s_Textures.at(s_CurrentTextureIndex);
    }

    sf::RenderTexture* Renderer::GetFinishedTexture()
    {
        uint32_t finishedTextureIndex = (s_CurrentTextureIndex + 1) % 2;
        return s_Textures.at(finishedTextureIndex);
    }

	void Renderer::CreateTextures(sf::Vector2u resize)
	{
		for (auto& texture : s_Textures)
		{
			texture->create(resize.x, resize.y);
			texture->clear();
			texture->display();
		}
	}

	void Renderer::SwapTextures()
    {
		s_CurrentTextureIndex = (s_CurrentTextureIndex + 1) % 2; // toggle between 0 and 1
    }

    void Renderer::BeginSceneDrawing()
    {
		s_Textures.at(s_CurrentTextureIndex)->clear();
    }

    void Renderer::BeginDrawing()
    {
		s_IsDrawing = true;
		s_DrawCalls = 0;
    }

    void Renderer::Draw(const sf::Shape& shape, sf::RenderTarget* renderTarget)
    {
        Draw(static_cast<const sf::Drawable&>(shape), sf::RenderStates::Default, renderTarget);
    }

    void Renderer::Draw(const sf::Sprite& sprite, sf::RenderTarget* renderTarget)
    {
        Draw(static_cast<const sf::Drawable&>(sprite), sf::RenderStates::Default,  renderTarget);
    }

    void Renderer::Draw(const sf::Drawable& drawable, const sf::RenderStates& state, sf::RenderTarget* renderTarget)
    {
        // look more into this section in the future
        if (renderTarget == nullptr)
        {
            renderTarget = s_Textures.at(s_CurrentTextureIndex);
        }

        renderTarget->draw(drawable, state);
        s_DrawCalls++;
    }

    void Renderer::DrawBufferTexture()
    {
        auto& window = Application::Get().GetWindow().GetRenderWindow();

		s_BufferSprite.setTexture(s_Textures.at(s_CurrentTextureIndex)->getTexture());
        Draw(s_BufferSprite, &window);
    }

    void Renderer::EndDrawing() { s_IsDrawing = false; }

    void Renderer::EndSceneDrawing()
    {
        SwapTextures();
    }

    bool Renderer::IsDrawing() { return s_IsDrawing; }

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, sf::Vector2f size, sf::Vector2f texCoord)
	{
		return CreateQuad(position, 0, size, texCoord, sf::Color(255, 255, 255));
	}

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, sf::Vector2f size, sf::Color colour)
	{
		return CreateQuad(position, 0, size, colour);
	}

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, sf::Vector2f size, sf::Vector2f texCoord, sf::Color colour)
	{
		return CreateQuad(position, 0, size, texCoord, colour);
	}

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoord)
	{
		return CreateQuad(position, rotation, size, texCoord, sf::Color(255, 255, 255));
	}

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Color colour)
	{
		return CreateQuad(position, rotation, size, sf::Vector2f(0, 0), colour);
	}

	std::array<sf::Vertex, 4> Renderer::CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoord, sf::Color colour)
	{
		std::array<sf::Vertex, 4> quad;
		sf::Vector2f center = position + size * 0.5f;
		sf::Transform transform;

		transform.translate(center);
		transform.rotate(rotation);
		transform.translate(-center);

		quad[0].position = transform.transformPoint(sf::Vector2f(position.x, position.y));
		quad[1].position = transform.transformPoint(sf::Vector2f(position.x + size.x, position.y));
		quad[2].position = transform.transformPoint(sf::Vector2f(position.x + size.x, position.y + size.y));
		quad[3].position = transform.transformPoint(sf::Vector2f(position.x, position.y + size.y));

		quad[0].texCoords = sf::Vector2f((texCoord.x + 0) * size.x, (texCoord.y + 0) * size.y);
		quad[1].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, (texCoord.y + 0) * size.y);
		quad[2].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, (texCoord.y + 1) * size.y);
		quad[3].texCoords = sf::Vector2f((texCoord.x + 0) * size.x, (texCoord.y + 1) * size.y);

		quad[0].color = colour;
		quad[1].color = colour;
		quad[2].color = colour;
		quad[3].color = colour;

		return quad;
	}

} // Maize