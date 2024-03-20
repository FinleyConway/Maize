#pragma once

namespace Maize {

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
		void Initialize(sf::RenderWindow& window); // initialize renderer
		void OnWindowResize(sf::Vector2f resize); // resizes the games view based on window size

		void SetClearColour(sf::Color clearColour); // set clear colour for background

		void InsertDrawable(const Transform& transform, SpriteRenderer& spriteRenderer); // insert a new drawable to the rendering buffer
		void RemoveDrawable(const sf::Drawable* drawable); // remove drawable from the rendering buffer
		void UpdateDrawable(const Transform& transform, SpriteRenderer& spriteRenderer); // update drawable

        void BeginDrawing(); // set rendering state to start drawing
		void DrawScene(); // draw rendering buffer to the screen
		void DrawImmediately(const sf::Drawable* drawable); // draw immediately to the screen
		void EndDrawing(); // set rendering state to end drawing

		bool IsDrawing() const; // is rendering in the drawing state?
		size_t GetDrawCall() const; // returns the amount of drawables being drawn

    private:
		bool InsideViewport(const RenderData& renderData); // checks if drawable is within the view

		std::vector<RenderData> m_Drawables;

		bool m_IsDrawing = false;
		size_t m_DrawCalls = 0;

		sf::RenderWindow* m_RenderWindow = nullptr;
		sf::View m_DefaultView;
		sf::Color m_ClearColour = { 70, 130, 180 };

		static constexpr float m_PixelPerUnit = 100.0f;
		static constexpr int8_t m_Flip = -1;
    };

} // Maize