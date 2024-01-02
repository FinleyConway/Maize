#pragma once

#include "Maize/Renderer/Texture.h"
#include "Maize/Math/Vector2.h"

namespace Maize {

	class Sprite : public sf::Drawable, private sf::Transformable
	{
	public:
		Sprite() = default;
		Sprite(const Texture& texture, const sf::IntRect& rectangle, Vector2 origin);
		Sprite(const Texture& texture, const sf::IntRect& rectangle, Vector2 origin, float pixelPerUnit);

		void SetTexture(const Texture& texture, bool resetRect = false);
		void SetTextureRect(const sf::IntRect& rectangle);
		void SetColour(sf::Color colour);

		const Texture* GetTexture() const;
		const sf::IntRect& GetTextureRect() const;
		sf::Color GetColour() const;
		float GetPixelPerUnit() const { return m_PixelPerUnit; }

		sf::FloatRect GetLocalBounds() const;
		sf::FloatRect GetGlobalBounds() const;

	private:
		friend class RenderingSystem;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void UpdatePositions();
		void UpdateTexCoords();

	private:
		std::array<sf::Vertex, 4> m_Vertices;
		const Texture* m_Texture = nullptr;
		sf::IntRect m_TextureRect;
		float m_PixelPerUnit = 100;
	};

} // Maize