#pragma once

#include "Maize/Math/Math.h"

namespace Maize {

	struct Vector2Int;

	struct Vector2
	{
	public:
		Vector2() = default;
		Vector2(float x, float y);
		Vector2(const Vector2& other) = default;
		explicit Vector2(const Vector2Int& other);

		float x = 0.0f;
		float y = 0.0f;

		static Vector2 Up();
		static Vector2 Down();
		static Vector2 Left();
		static Vector2 Right();
		static Vector2 PositiveInfinity();
		static Vector2 NegativeInfinity();
		static Vector2 One();
		static Vector2 Zero();

		float Magnitude() const;
		Vector2 Normalized() const;
		void Normalize();
		float SqrMagnitude() const;

		static float Angle(Vector2 from, Vector2 to);
		static Vector2 ClampMagnitude(Vector2 vector, float maxLength);
		static float Distance(Vector2 a, Vector2 b);
		static float Dot(Vector2 a, Vector2 b);
		static Vector2 Lerp(Vector2 a, Vector2 b, float time);
		static Vector2 LerpClamped(Vector2 a, Vector2 b, float time);
		static Vector2 SmoothDamp(Vector2 current, Vector2 target, Vector2& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
		static Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta);
		static Vector2 Max(Vector2 a, Vector2 b);
		static Vector2 Min(Vector2 a, Vector2 b);
		static Vector2 Perpendicular(Vector2 vector);
		static Vector2 Reflect(Vector2 inDirection, Vector2 inNormal);
		static float SignedAngle(Vector2 from, Vector2 to);

		Vector2 operator-(const Vector2& other) const;
		Vector2 operator+(const Vector2& other) const;
		Vector2& operator+=(const Vector2& other)
		{
			x += other.x;
			y += other.y;
			return *this;
		}
		Vector2 operator*(float scalar) const;
		friend Vector2 operator*(float scalar, const Vector2& vec);
		Vector2 operator/(float scalar) const;
		friend Vector2 operator/(float scalar, const Vector2& vec);
		bool operator==(const Vector2& other) const;
		friend std::ostream& operator<<(std::ostream& os, const Vector2& vector);
	};
}
