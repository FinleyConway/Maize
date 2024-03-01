#pragma once

namespace Maize {

	class Sprite;
	struct Transform;
    struct SpriteRenderer;

	struct RenderData
	{
		const sf::Drawable* drawable = nullptr;
		sf::FloatRect bounds;
		int32_t sortingLayer = 0;
		int32_t orderInLayer = 0;

		bool operator<(const RenderData& other) const
		{
			if (sortingLayer < other.sortingLayer)
			{
				return true;
			}
			else if (sortingLayer == other.sortingLayer)
			{
				return orderInLayer < other.orderInLayer;
			}

			return false;
		}
	};

    class Renderer
    {
    public:
		static void Initialize(sf::RenderWindow& window);

		static void OnWindowResize(sf::Vector2f resize);

		static void InsertDrawable(const Transform& transform, SpriteRenderer& spriteRenderer);
		static void RemoveDrawable(const sf::Drawable* drawable);
		static void UpdateDrawable(const Transform& transform, SpriteRenderer& spriteRenderer);

        static void BeginDrawing(sf::Color clearColour);

		static void DrawScene();
		static void DrawImmediately(const sf::Drawable* drawable);

		static void EndDrawing();

		static bool IsDrawing() { return s_IsDrawing; };
		static size_t GetDrawCall() { return s_DrawCalls; }

    private:
		static bool InsideViewport(const RenderData& renderData);

		inline static std::vector<RenderData> s_Drawables;

		inline static bool s_IsDrawing = false;
		inline static size_t s_DrawCalls = 0;

		inline static sf::RenderWindow* s_RenderWindow = nullptr;
		inline static sf::View s_DefaultView;

		static constexpr float m_PixelPerUnit = 100.0f;
		static constexpr int8_t m_Flip = -1;
    };

} // Maize