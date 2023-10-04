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
        void Draw(const sf::Drawable& drawable, const sf::RenderStates& state = sf::RenderStates::Default, sf::RenderTarget* renderTarget = nullptr);
        void DrawBufferTexture();

        void EndDrawing();
        void EndSceneDrawing();

        bool IsDrawing() const;

		static std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoord)
		{
			std::array<sf::Vertex, 4> quad;

			sf::Vector2f rotatedPosition = sf::Vector2f(position.x * std::cos(rotation * 3.14f / 180), position.y + std::sin(rotation * 3.14f / 180));

			quad[0].position = sf::Vector2f((rotatedPosition.x + 0) * size.x, (rotatedPosition.y + 0) * size.y);
			quad[1].position = sf::Vector2f((rotatedPosition.x + 1) * size.x, (rotatedPosition.y + 0) * size.y);
			quad[2].position = sf::Vector2f((rotatedPosition.x + 1) * size.x, (rotatedPosition.y + 1) * size.y);
			quad[3].position = sf::Vector2f((rotatedPosition.x + 0) * size.x, (rotatedPosition.y + 1) * size.y);

			quad[0].texCoords = sf::Vector2f((texCoord.x + 0) * size.x, (texCoord.y + 0) * size.y);
			quad[1].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, (texCoord.y + 0) * size.y);
			quad[2].texCoords = sf::Vector2f((texCoord.x + 1) * size.x, (texCoord.y + 1) * size.y);
			quad[3].texCoords = sf::Vector2f((texCoord.x + 0) * size.x, (texCoord.y + 1) * size.y);

			return quad;
		}

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