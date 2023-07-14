#pragma once

#include <SDL.h>
#include <cmath>

namespace Maize {

	struct PointF
	{
	public:
		PointF() = default;
		PointF(float x, float y);
		PointF(SDL_FPoint point);

		float x = 0;
		float y = 0;

		const static PointF one;
		const static PointF zero;
		const static PointF up;
		const static PointF down;
		const static PointF left;
		const static PointF right;

		static float Angle(PointF from, PointF to);
		static float Distance(PointF a, PointF b);
		static float Dot(PointF other1, PointF other2);
		static PointF ClampMagnitude(PointF other, float maxLength);
		static PointF Lerp(PointF a, PointF b, float t);
		static PointF Max(PointF other1, PointF other2);
		static PointF Min(PointF other1, PointF other2);
		static PointF Reflect(PointF inDirection, PointF inNormal);

		float Magnitude() const;
		PointF Normalized() const;

		operator SDL_FPoint() const;
		operator const SDL_FPoint* () const;

		PointF operator+(PointF other) const;
		PointF operator-(PointF other) const;
		PointF operator*(float scalar) const;
		PointF operator/(float scalar) const;

		PointF& operator+=(PointF other);
		PointF& operator-=(PointF other);
		PointF& operator*=(float scalar);
		PointF& operator/=(float scalar);

		bool operator==(PointF other) const;
	};

}