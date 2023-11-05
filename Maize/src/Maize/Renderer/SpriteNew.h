#pragma once

#include "Texture.h"

namespace Maize {

	class SpriteNew : public sf::Drawable
	{
	public:
		explicit SpriteNew(const std::shared_ptr<Texture>& texture);
		SpriteNew(const std::shared_ptr<Texture>& texture, const sf::IntRect& rectangle);

		void SetPosition(sf::Vector2f position);
		void SetRotation(float angle);
		void SetScale(sf::Vector2f scale);
		void SetOrigin(sf::Vector2f origin);

		void SetTexture(const std::shared_ptr<Texture>& texture, bool resetRect = false);
		void SetTextureRect(const sf::IntRect& rectangle);
		void SetColour(sf::Color colour);

		void GetPosition(sf::Vector2f position);
		void GetRotation(float angle);
		void GetScale(sf::Vector2f scale);
		void GetOrigin(sf::Vector2f origin);

		const Texture& GetTexture() const;
		const sf::IntRect& GetTextureRect() const;
		sf::Color GetColour() const;

		sf::FloatRect GetLocalBounds() const;
		sf::FloatRect GetGlobalBounds() const;

	private:
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		void UpdatePositions();
		void UpdateTexCoords();

	private:
		std::array<sf::Vertex, 4> m_Vertices;
		std::shared_ptr<Texture> m_Texture;
		sf::IntRect m_TextureRect;
		sf::Transformable m_Transform;
	};

} // Maize