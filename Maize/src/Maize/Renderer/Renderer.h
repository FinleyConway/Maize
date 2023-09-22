#pragma once

namespace Maize {

    class Window;

    class Renderer
    {
    public:
        explicit Renderer(Window& window);

        sf::RenderTexture* GetCurrentTexture();
        sf::RenderTexture* GetFinishedTexture();

        void BeginSceneDrawing();
        void BeginDrawing();

        void Draw(const sf::Shape& shape, sf::RenderTarget* renderTarget = nullptr);
        void Draw(const sf::Sprite& sprite, sf::RenderTarget* renderTarget = nullptr);
        void Draw(const sf::Drawable& drawable, sf::RenderTarget* renderTarget = nullptr);
        void DrawBufferTexture();

        void EndDrawing();
        void EndSceneDrawing();

        bool IsDrawing() const;

    private:
        void CreateTextures();
        void SwapTextures();

    private:
        friend class Window;

        Window& m_Window;

        sf::RenderTexture m_Textures[2];
        sf::Sprite m_BufferSprite;

        bool m_IsDrawing = false;
        uint32_t m_DrawCalls = 0;
        uint32_t m_CurrentTextureIndex = 0;
    };

} // Maize