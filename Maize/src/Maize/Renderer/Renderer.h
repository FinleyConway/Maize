#pragma once

namespace Maize {

    class Renderer
    {
    public:
		static void Initialize(sf::Vector2u resize);
		static void Shutdown();

		static void OnWindowResize(sf::Vector2u resize);

        static sf::RenderTexture* GetCurrentTexture();
        static sf::RenderTexture* GetFinishedTexture();

        static void BeginSceneDrawing();
        static void BeginDrawing();

        static void Draw(const sf::Shape& shape, sf::RenderTarget* renderTarget = nullptr);
        static void Draw(const sf::Sprite& sprite, sf::RenderTarget* renderTarget = nullptr);
        static void Draw(const sf::Drawable& drawable, const sf::RenderStates& state = sf::RenderStates::Default, sf::RenderTarget* renderTarget = nullptr);
        static void DrawBufferTexture();

        static void EndDrawing();
        static void EndSceneDrawing();

        static bool IsDrawing();
		static uint32_t GetDrawCall() { return s_DrawCalls; };

		static std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, sf::Vector2f size, sf::Vector2f texCoord);
		static std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, sf::Vector2f size, sf::Color colour);
		static std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, sf::Vector2f size, sf::Vector2f texCoord, sf::Color colour);
		static std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoord);
		static std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Color colour);
		static std::array<sf::Vertex, 4> CreateQuad(sf::Vector2f position, float rotation, sf::Vector2f size, sf::Vector2f texCoord, sf::Color colour);

    private:
        static void CreateTextures(sf::Vector2u resize);
        static void SwapTextures();

    private:
		inline static sf::Sprite s_BufferSprite;
		inline static std::array<sf::RenderTexture*, 2> s_Textures;

        inline static bool s_IsDrawing = false;
        inline static uint32_t s_DrawCalls = 0;
        inline static uint32_t s_CurrentTextureIndex = 0;
    };

} // Maize