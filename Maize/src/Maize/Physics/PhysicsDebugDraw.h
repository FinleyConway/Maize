#pragma once

#include <box2d/box2d.h>

namespace Maize {

	class PhysicsDebugDraw : public b2Draw
	{
	 public:
		void SetRenderTarget(sf::RenderTarget& target);

		void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
		void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) override;
		void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color) override;
		void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color) override;
		void DrawPoint(const b2Vec2 &p, float size, const b2Color &color) override;
		void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) override;
		void DrawTransform(const b2Transform &xf) override;

	 private:
		static sf::Color ConvertColourB2ToSf(b2Color c)
		{
			return { static_cast<uint8_t>(c.r * 255),static_cast<uint8_t>(c.g * 255),static_cast<uint8_t>(c.b * 255), static_cast<uint8_t>(c.a * 255) };
		}

	 private:
		sf::RenderTarget* m_Target;

		static constexpr float m_PixelPerUnit = 100.0f;
		static constexpr int8_t m_Flip = -1;
	};

} // Maize
