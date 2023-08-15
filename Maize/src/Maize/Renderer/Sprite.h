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
		Sprite(const Rect& rect, PointF pivot, const Texture* texture) :
			m_Pivot(pivot),
			m_Texture(texture)
		{
			if (texture != nullptr)
			{
				m_Sprite.setTextureRect(rect);
				m_Sprite.setTexture(*texture);
			}
		}

		void SetColour(Colour colour) { m_Sprite.setColor(colour); }
		void SetPosition(PointF position) { m_Sprite.setPosition(position); }

		Rect GetTexturePosition() const { return m_Sprite.getTextureRect(); }
		PointF GetPivot() const { return m_Pivot; }
		const Texture* GetTexture() const { return m_Texture; }
		Colour GetColour() const { return m_Sprite.getColor(); }

		operator const sf::Sprite& () const { return m_Sprite; }

	private:
		PointF m_Pivot = PointF(0, 0);
		const Texture* m_Texture = nullptr;
		sf::Sprite m_Sprite;
	};

}