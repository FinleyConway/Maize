#include "mpch.h"
#include "Maize/Renderer/Renderer.h"
#include "Maize/Core/Window.h"

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

        Draw(static_cast<const sf::Drawable&>(shape), renderTarget);
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

        Draw(static_cast<const sf::Drawable&>(sprite), renderTarget);
    }

    void Renderer::Draw(const sf::Drawable& drawable, sf::RenderTarget* renderTarget)
    {
        // look more into this section in the future
        if (renderTarget == nullptr)
        {
            //renderTarget = &m_Textures[m_CurrentTextureIndex];
            renderTarget = &m_Window.GetRenderWindow();
        }

        renderTarget->draw(drawable);
        m_DrawCalls++;
    }

    void Renderer::DrawBufferTexture()
    {
        m_BufferSprite.setTexture(m_Textures[m_CurrentTextureIndex].getTexture());
        Draw(m_BufferSprite);
    }

    void Renderer::EndDrawing() { m_IsDrawing = false; }

    void Renderer::EndSceneDrawing()
    {
        SwapTextures();
    }

    bool Renderer::IsDrawing() const { return m_IsDrawing; }

} // Maize