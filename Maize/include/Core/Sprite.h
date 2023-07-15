#pragma once

#include <SDL.h>
#include <string>

#include "Texture.h"
#include "Point.h"
#include "Rect.h"

namespace Maize {

	struct Sprite
	{
	public:
		Sprite() = default;
		Sprite(const std::string& name, const Rect& position, PointF pivot, float ppu, Texture& texture) :
			m_Name(name), m_Position(position), m_Pivot(pivot), m_PixelPerUnit(ppu), m_Texture(texture) { }

		const std::string& Name() const { return m_Name; }
		Rect Position() const { return m_Position; }
		PointF Pivot() const { return m_Pivot; }
		float PPU() const { return m_PixelPerUnit; }
		Texture& Tex() const { return m_Texture; }

	private:
		std::string m_Name = "";
		Rect m_Position = { 0, 0, 0, 0 };
		PointF m_Pivot = { 0, 0 };
		float m_PixelPerUnit = 100;
		Texture& m_Texture;
	};

}