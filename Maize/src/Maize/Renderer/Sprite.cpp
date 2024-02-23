#include "mpch.h"
#include "Maize/Renderer/Sprite.h"
#include "Maize/Renderer/Texture.h"

#include "Maize/Math/Vector2.h"

namespace Maize {

	Sprite::Sprite(const Texture& texture, const sf::IntRect& rectangle, Vector2 origin)
	{
		SetTextureRect(rectangle);
		SetTexture(texture, false);
		setOrigin(origin.x, origin.y);
	}

	Sprite::Sprite(const Texture& texture, const sf::IntRect& rectangle, Vector2 origin, float pixelPerUnit)
	{
		SetTextureRect(rectangle);
		SetTexture(texture, false);
		setOrigin(origin.x, origin.y);

		float scale = m_PixelPerUnit / pixelPerUnit;
		setScale(scale, scale);

		m_PixelPerUnit = pixelPerUnit;
	}

	void Sprite::SetTexture(const Texture& texture, bool resetRect)
	{
		if (resetRect || (m_Texture == nullptr && m_TextureRect == sf::IntRect()))
		{
			sf::Vector2i size = sf::Vector2i(texture.GetSize());
			SetTextureRect(sf::IntRect(0, 0, size.x, size.y));
		}

		m_Texture = &texture;
	}

	void Sprite::SetTextureRect(const sf::IntRect& rectangle)
	{
		if (rectangle != m_TextureRect)
		{
			m_TextureRect = rectangle;
			UpdatePositions();
			UpdateTexCoords();
		}
	}

	void Sprite::SetColour(sf::Color colour)
	{
		for (auto& vertex : m_Vertices)
		{
			vertex.color = colour;
		}
	}

	const Texture* Sprite::GetTexture() const
	{
		return m_Texture;
	}

	const sf::IntRect& Sprite::GetTextureRect() const
	{
		return m_TextureRect;
	}

	sf::Color Sprite::GetColour() const
	{
		return m_Vertices[0].color;
	}

	sf::FloatRect Sprite::GetLocalBounds() const
	{
		const auto width  = static_cast<float>(std::abs(m_TextureRect.width));
		const auto height = static_cast<float>(std::abs(m_TextureRect.height));

		return { { 0.f, 0.f }, { width, height } };
	}

	sf::FloatRect Sprite::GetGlobalBounds() const
	{
		return getTransform().transformRect(GetLocalBounds());
	}

	void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (m_Texture != nullptr)
		{
			states.transform *= getTransform();
			states.texture = &m_Texture->GetTexture();
			target.draw(m_Vertices.data(), 4, sf::PrimitiveType::TriangleStrip, states);
		}
	}

	void Sprite::UpdatePositions()
	{
		const sf::FloatRect bounds = GetLocalBounds();

		m_Vertices[0].position = sf::Vector2f(0, 0);
		m_Vertices[1].position = sf::Vector2f(0, bounds.height);
		m_Vertices[2].position = sf::Vector2f(bounds.width, 0);
		m_Vertices[3].position = sf::Vector2f(bounds.width, bounds.height);
	}

	void Sprite::UpdateTexCoords()
	{
		const sf::FloatRect convertedTextureRect(m_TextureRect);

		const float left = convertedTextureRect.left;
		const float right = left + convertedTextureRect.width;
		const float top = convertedTextureRect.top;
		const float bottom = top + convertedTextureRect.height;

		m_Vertices[0].texCoords = sf::Vector2f(left, top);
		m_Vertices[1].texCoords = sf::Vector2f(left, bottom);
		m_Vertices[2].texCoords = sf::Vector2f(right, top);
		m_Vertices[3].texCoords = sf::Vector2f(right, bottom);
	}

} // Maize