#include "mpch.h"
#include "Maize/Math/Vector2Int.h"
#include "Maize/Math/Vector2.h"

namespace Maize {

	Vector2Int::Vector2Int(int32_t x, int32_t y) : x(x), y(y)
	{
	}

	Vector2Int Vector2Int::Up()
	{
		return { 0, 1 };
	}

	Vector2Int Vector2Int::Down()
	{
		return { 0, -1 };
	}

	Vector2Int Vector2Int::Left()
	{
		return { -1, 0 };
	}

	Vector2Int Vector2Int::Right()
	{
		return { 1, 0 };
	}

	Vector2Int Vector2Int::One()
	{
		return { 1, 1 };
	}

	Vector2Int Vector2Int::Zero()
	{
		return { 0, 0 };
	}

	float Vector2Int::Magnitude() const
	{
		return Math::Sqrt(x * x + y * y);
	}

	float Vector2Int::SqrMagnitude() const
	{
		return x * x + y * y;
	}

	void Vector2Int::Clamp(Vector2Int min, Vector2Int max)
	{
		x = Math::Clamp(x, min.x, max.x);
		y = Math::Clamp(y, min.y, max.y);
	}

	Vector2Int Vector2Int::CeilToInt(Vector2 v)
	{
		int32_t cX = Math::CeilToInt(v.x);
		int32_t cY = Math::CeilToInt(v.y);

		return { cX, cY };
	}

	Vector2Int Vector2Int::FloorToInt(Vector2 v)
	{
		int32_t cX = Math::FloorToInt(v.x);
		int32_t cY = Math::FloorToInt(v.y);

		return { cX, cY };
	}

	Vector2Int Vector2Int::RoundToInt(Vector2 v)
	{
		int32_t cX = Math::RoundToInt(v.x);
		int32_t cY = Math::RoundToInt(v.y);

		return { cX, cY };
	}

	float Vector2Int::Distance(Vector2Int a, Vector2Int b)
	{
		int32_t dx = b.x - a.x;
		int32_t dy = b.y - a.y;

		return Math::Sqrt(dx * dx + dy * dy);
	}

	Vector2Int Vector2Int::Max(Vector2Int a, Vector2Int b)
	{
		return { Math::Max(a.x, b.x), Math::Max(a.y, b.y) };
	}

	Vector2Int Vector2Int::Min(Vector2Int a, Vector2Int b)
	{
		return { Math::Min(a.x, b.x), Math::Min(a.y, b.y) };
	}

	Vector2Int Vector2Int::operator-(const Vector2Int& other) const
	{
		return Vector2Int(x - other.x, y - other.y);
	}

	Vector2Int Vector2Int::operator+(const Vector2Int& other) const
	{
		return Vector2Int(x + other.x, y + other.y);
	}

	Vector2Int Vector2Int::operator*(const Vector2Int& other) const
	{
		return Vector2Int(x * other.x, y * other.y);
	}

	Vector2Int Vector2Int::operator*(int32_t scalar) const
	{
		return Vector2Int(scalar * x, scalar * y);
	}

	Vector2Int Vector2Int::operator/(int32_t divisor) const
	{
		if (divisor != 0)
		{
			return Vector2Int(x / divisor, y / divisor);
		}

		return Vector2Int::Zero();
	}

	bool Vector2Int::operator==(const Vector2Int& other) const
	{
		return x == other.x && y == other.y;
	}

	std::ostream& operator<<(std::ostream& os, const Vector2Int& vector)
	{
		os << "(" << vector.x << ", " << vector.y << ")";
		return os;
	}

}