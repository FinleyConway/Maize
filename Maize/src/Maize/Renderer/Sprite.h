#pragma once

#include "Maize/Renderer/Texture.h"

namespace Maize {

	class Sprite : public sf::Drawable, private sf::Transformable
	{
	public:
		Sprite() = default;
		explicit Sprite(const Texture& texture);
		Sprite(const Texture& texture, const sf::IntRect& rectangle);
		Sprite(const Texture& texture, const sf::IntRect& rectangle, sf::Vector2f origin);

		void SetTexture(const Texture& texture, bool resetRect = false);
		void SetTextureRect(const sf::IntRect& rectangle);
		void SetColour(sf::Color colour);

		const Texture* GetTexture() const;
		const sf::IntRect& GetTextureRect() const;
		sf::Color GetColour() const;

		sf::FloatRect GetLocalBounds() const;
		sf::FloatRect GetGlobalBounds() const;

		void FlipX(bool flip);
		void FlipY(bool flip);

		bool IsFlippedX() const;
		bool IsFlippedY() const;

	private:
		friend class RenderingSystem;

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void UpdatePositions();
		void UpdateTexCoords();

	private:
		std::array<sf::Vertex, 4> m_Vertices;
		const Texture* m_Texture = nullptr;
		sf::IntRect m_TextureRect;
		bool m_FlippedX = false;
		bool m_FlippedY = false;
	};

} // Maize