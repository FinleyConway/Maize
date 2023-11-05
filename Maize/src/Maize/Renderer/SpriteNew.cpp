//
// Created by finley on 28/10/23.
//

#include "SpriteNew.h"

namespace Maize {

	SpriteNew::SpriteNew(const std::shared_ptr<Texture>& texture)
	{
		SetTexture(texture, true);
	}

	SpriteNew::SpriteNew(const std::shared_ptr<Texture>& texture, const sf::IntRect& rectangle)
	{
		SetTextureRect(rectangle);
		SetTexture(texture, false);
	}

	void SpriteNew::SetTexture(const std::shared_ptr<Texture>& texture, bool resetRect)
	{
		if (resetRect)
		{
			SetTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(texture->GetSize())));
		}

		m_Texture = texture;
	}

	void SpriteNew::SetTextureRect(const sf::IntRect& rectangle)
	{
		if (rectangle != m_TextureRect)
		{
			m_TextureRect = rectangle;
			UpdatePositions();
			UpdateTexCoords();
		}
	}

	void SpriteNew::SetColour(sf::Color colour)
	{
		for (auto& vertex : m_Vertices)
		{
			vertex.color = colour;
		}
	}

	const Texture& SpriteNew::GetTexture() const
	{
		return *m_Texture;
	}

	const sf::IntRect& SpriteNew::GetTextureRect() const
	{
		return m_TextureRect;
	}

	sf::Color SpriteNew::GetColour() const
	{
		return m_Vertices[0].color;
	}

	sf::FloatRect SpriteNew::GetLocalBounds() const
	{
		const auto width  = static_cast<float>(std::abs(m_TextureRect.width));
		const auto height = static_cast<float>(std::abs(m_TextureRect.height));

		return { { 0.f, 0.f }, { width, height } };
	}

	sf::FloatRect SpriteNew::GetGlobalBounds() const
	{
		return m_Transform.getTransform().transformRect(GetLocalBounds());
	}

	void SpriteNew::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::RenderStates statesCopy(states);

		statesCopy.transform *= m_Transform.getTransform();
		statesCopy.texture = &m_Texture->GetTexture();
		target.draw(m_Vertices.data(), 4, sf::PrimitiveType::TriangleStrip, states);
	}

	void SpriteNew::UpdatePositions()
	{
		const sf::FloatRect bounds = GetLocalBounds();

		m_Vertices[0].position = sf::Vector2f(0, 0);
		m_Vertices[1].position = sf::Vector2f(0, bounds.height);
		m_Vertices[2].position = sf::Vector2f(bounds.width, 0);
		m_Vertices[3].position = sf::Vector2f(bounds.width, bounds.height);
	}

	void SpriteNew::UpdateTexCoords()
	{
		const sf::FloatRect convertedTextureRect(m_TextureRect);

		const float left   = convertedTextureRect.left;
		const float right  = left + convertedTextureRect.width;
		const float top    = convertedTextureRect.top;
		const float bottom = top + convertedTextureRect.height;

		m_Vertices[0].texCoords = sf::Vector2f(left, top);
		m_Vertices[1].texCoords = sf::Vector2f(left, bottom);
		m_Vertices[2].texCoords = sf::Vector2f(right, top);
		m_Vertices[3].texCoords = sf::Vector2f(right, bottom);
	}

} // Maize