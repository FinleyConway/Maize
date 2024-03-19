#include "mpch.h"
#include "Maize/Physics/PhysicsDebugDraw.h"

namespace Maize {

	void PhysicsDebugDraw::SetRenderTarget(sf::RenderTarget& target)
	{
		m_Target = &target;
	}

	void PhysicsDebugDraw::DrawPolygon(const b2Vec2* vertices, int32_t vertexCount, const b2Color& colour) { }

	void PhysicsDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32_t vertexCount, const b2Color& colour)
	{
		auto shape = sf::ConvexShape(vertexCount);

		for (auto i = 0; i < vertexCount; ++i)
		{
			shape.setPoint(i, { vertices[i].x * m_PixelPerUnit, vertices[i].y * m_PixelPerUnit * m_Flip });
		}

		shape.setFillColor(sf::Color::Transparent);
		shape.setOutlineColor(ConvertColourB2ToSf(colour));
		shape.setOutlineThickness(1.0f);

		m_Target->draw(shape);
	}

	void PhysicsDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& colour) { }

	void PhysicsDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& colour)
	{
		auto circle = sf::CircleShape(radius);

		circle.setPosition(center.x * m_PixelPerUnit, center.y * m_PixelPerUnit * m_Flip);
		circle.setOrigin(radius / 2.0f, radius / 2.0f);

		circle.setOutlineColor(ConvertColourB2ToSf(colour));
		circle.setOutlineThickness(1.0f);

		m_Target->draw(circle);

		auto p = center + (radius * axis);

		DrawSegment(center, p, colour);
	}

	void PhysicsDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& colour)
	{
		auto circle = sf::CircleShape(size);

		circle.setPosition(p.x * m_PixelPerUnit, p.y * m_PixelPerUnit * m_Flip);
		circle.setOrigin(size / 2.0f, size / 2.0f);
		circle.setFillColor(ConvertColourB2ToSf(colour));

		m_Target->draw(circle);
	}

	void PhysicsDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& colour)
	{
		auto vertexArray = sf::VertexArray(sf::Lines, 2);

		vertexArray[0].position = { p1.x * m_PixelPerUnit, p1.y * m_PixelPerUnit * m_Flip };
		vertexArray[0].color = ConvertColourB2ToSf(colour);

		vertexArray[1].position = { p2.x * m_PixelPerUnit, p2.y * m_PixelPerUnit * m_Flip };
		vertexArray[1].color = ConvertColourB2ToSf(colour);

		m_Target->draw(vertexArray);
	}

	void PhysicsDebugDraw::DrawTransform(const b2Transform& xf)
	{
		auto positionX = xf.p + (0.5f * xf.q.GetXAxis());
		auto positionY = xf.p + (0.5f * xf.q.GetYAxis());

		DrawSegment(xf.p, positionX, { 255.0f, 0.0f, 0.0f });
		DrawSegment(xf.p, positionY, { 255.0f, 0.0f, 0.0f });
	}

} // Maize