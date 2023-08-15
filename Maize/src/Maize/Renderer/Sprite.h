#pragma once

#include "Maize/Renderer/Texture.h"
#include "Maize/Renderer/Colour.h"
#include "Maize/Math/PointF.h"
#include "Maize/Math/Rect.h"

namespace Maize {

	struct Sprite
	{
	public:
		Sprite() = default;
		Sprite(const Rect& rect, PointF pivot, Texture& texture) :
			m_Pivot(pivot),
			m_Texture(texture)
		{
			m_Sprite.setTextureRect(rect);
		}

		void SetColour(Colour colour) { m_Sprite.setColor(colour); }

		Rect GetTexturePosition() const { return m_Sprite.getTextureRect(); }
		PointF GetPivot() const { return m_Pivot; }
		const Texture& GetTexture() const { return m_Texture; }
		Colour GetColour() const { return m_Sprite.getColor(); }

		operator const sf::Sprite& () const { return m_Sprite; }

	private:
		PointF m_Pivot = { 0, 0 };
		Texture& m_Texture;
		sf::Sprite m_Sprite;
	};

}