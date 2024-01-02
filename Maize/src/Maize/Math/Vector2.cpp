#include "mpch.h"
#include "Maize/Math/Vector2.h"

#include "Maize/Math/Vector2Int.h"
#include "Maize/Math/Math.h"

namespace Maize {

	Vector2::Vector2(float x, float y) : x(x), y(y)
	{
	}

	Vector2::Vector2(const Vector2Int& other) : x(static_cast<float>(other.x)), y(static_cast<float>(other.y))
	{
	}

	Vector2 Vector2::Up()
	{
		return { 0.0f, 1.0f };
	}

	Vector2 Vector2::Down()
	{
		return { 0.0f, -1.0f };
	}

	Vector2 Vector2::Left()
	{
		return { -1.0f, 0.0f };
	}

	Vector2 Vector2::Right()
	{
		return { 1.0f, 0.0f };
	}

	Vector2 Vector2::PositiveInfinity()
	{
		return { Math::Infinity(), Math::Infinity() };
	}

	Vector2 Vector2::NegativeInfinity()
	{
		return { Math::NegativeInfinity(), Math::NegativeInfinity() };
	}

	Vector2 Vector2::One()
	{
		return { 1.0f, 1.0f };
	}

	Vector2 Vector2::Zero()
	{
		return { 0.0f, 0.0f };
	}

	float Vector2::Magnitude() const
	{
		return Math::Sqrt(x * x + y * y);
	}

	Vector2 Vector2::Normalized() const
	{
		float mag = Magnitude();

		if (mag != 0)
		{
			return { x / mag, y / mag };
		}

		return Vector2::Zero();
	}

	void Vector2::Normalize()
	{
		float mag = Magnitude();

		if (mag != 0)
		{
			x /= mag;
			y /= mag;

			return;
		}

		x = 0;
		y = 0;
	}

	float Vector2::SqrMagnitude() const
	{
		return x * x + y * y;
	}

	float Vector2::Angle(Vector2 from, Vector2 to)
	{
		float dotProduct = Dot(from, to);
		float magnitudeProduct = from.Magnitude() * to.Magnitude();

		// handle special cases to avoid division by zero
		if (magnitudeProduct == 0)
		{
			return 0.0f;
		}

		float cosTheta = dotProduct / magnitudeProduct;

		return Math::Acos(Math::Clamp(cosTheta, -1.0f, 1.0f)) * Math::Rad2Deg();
	}

	Vector2 Vector2::ClampMagnitude(Vector2 vector, float maxLength)
	{
		float currentMagnitude = vector.Magnitude();

		if (currentMagnitude != 0.0f && currentMagnitude > maxLength)
		{
			return vector * (maxLength / currentMagnitude);
		}

		return vector;
	}

	float Vector2::Distance(Vector2 a, Vector2 b)
	{
		float dx = b.x - a.x;
		float dy = b.y - a.y;

		return Math::Sqrt(dx * dx + dy * dy);
	}

	float Vector2::Dot(Vector2 a, Vector2 b)
	{
		return a.x * b.x + a.y * b.y;
	}

	Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float time)
	{
		time = Math::Clamp(time, 0.0f, 1.0f);

		float x = Math::Lerp(a.x, b.x, time);
		float y = Math::Lerp(a.y, b.y, time);

		return { x, y };
	}

	Vector2 Vector2::LerpClamped(Vector2 a, Vector2 b, float time)
	{
		float x = Math::Lerp(a.x, b.x, time);
		float y = Math::Lerp(a.y, b.y, time);

		return { x, y };
	}

	Vector2 Vector2::SmoothDamp(Vector2 current, Vector2 target, Vector2& currentVelocity, float smoothTime, float maxSpeed, float deltaTime)
	{
		smoothTime = Math::Max(0.0001f, smoothTime);
		float omega = 2.0f / smoothTime;

		float x = omega * deltaTime;
		float exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
		Vector2 change = (current - target) * (1.0f - exp);
		Vector2 result = current - change;
		currentVelocity = (currentVelocity - change) * exp;

		// ensure we don't exceed the max speed
		if (maxSpeed > 0.0f)
		{
			float resultMagnitude = result.Magnitude();

			if (resultMagnitude > maxSpeed)
			{
				result = result.Normalized() * maxSpeed;
			}
		}

		return result;
	}

	Vector2 Vector2::MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
	{
		Vector2 toTarget = target - current;
		float distance = toTarget.Magnitude();

		if (distance <= maxDistanceDelta || distance == 0.0f)
		{
			return target;
		}

		return current + toTarget / distance * maxDistanceDelta;
	}

	Vector2 Vector2::Max(Vector2 a, Vector2 b)
	{
		return { Math::Max(a.x, b.x), Math::Max(a.y, b.y) };
	}

	Vector2 Vector2::Min(Vector2 a, Vector2 b)
	{
		return { Math::Min(a.x, b.x), Math::Min(a.y, b.y) };
	}

	Vector2 Vector2::Perpendicular(Vector2 vector)
	{
		return { -vector.y, vector.x };
	}

	Vector2 Vector2::Reflect(Vector2 inDirection, Vector2 inNormal)
	{
		return inDirection - 2.0f * Dot(inDirection, inNormal) * inNormal;
	}

	float Vector2::SignedAngle(Vector2 from, Vector2 to)
	{
		float unsignedAngle = Angle(from, to);

		// Cross product to determine the sign of the angle
		float cross = from.x * to.y - from.y * to.x;
		return cross >= 0.0f ? unsignedAngle : -unsignedAngle;
	}

	Vector2 Vector2::operator-(const Vector2& other) const
	{
		return { x - other.x, y - other.y };
	}

	Vector2 Vector2::operator+(const Vector2& other) const
	{
		return { x + other.x, y + other.y };
	}

	Vector2 Vector2::operator*(float scalar) const
	{
		return { x * scalar, y * scalar };
	}

	Vector2 operator*(float scalar, const Vector2& vec)
	{
		return { vec.x * scalar, vec.y * scalar };
	}

	Vector2 Vector2::operator/(float scalar) const
	{
		if (scalar != 0.0f)
		{
			return { x / scalar, y / scalar };
		}

		return Vector2::Zero();
	}

	Vector2 operator/(float scalar, const Vector2& vec)
	{
		if (scalar != 0.0f)
		{
			return { vec.x / scalar, vec.y / scalar };
		}

		return Vector2::Zero();
	}

	bool Vector2::operator==(const Vector2& other) const
	{
		bool approxX = Math::Approximately(x, other.x);
		bool approxY = Math::Approximately(y, other.y);

		return approxX && approxY;
	}

	std::ostream& operator<<(std::ostream& os, const Vector2& vector)
	{
		os << vector.x << ", " << vector.y;
		return os;
	}

}