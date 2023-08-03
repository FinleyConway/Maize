#pragma once

#include <SDL.h>
#include <cmath>

#include "Maize/Math/PointF.h"

#define intCast(value) static_cast<int32_t>(value)
#define floatCast(value) static_cast<float>(value)

namespace Maize {

	struct Point
	{
	public:
		Point() = default;
		Point(int32_t x, int32_t y);
		Point(SDL_Point point);

		int32_t x = 0;
		int32_t y = 0;

		const static Point one;
		const static Point zero;
		const static Point up;
		const static Point down;
		const static Point left;
		const static Point right;

		void Clamp(Point min, Point max);
		float Magnitude() const;

		static float Distance(Point a, Point b);
		static Point CeilToInt(Point other);
		static Point FloorToInt(Point other);
		static Point RoundToInt(Point other);
		static Point Max(Point other1, Point other2);
		static Point Min(Point other1, Point other2);

		operator SDL_Point() const;
		operator const SDL_Point* () const;
		operator PointF() const;

		Point operator+(Point other) const;
		Point operator-(Point other) const;
		Point operator*(float scalar) const;
		Point operator/(float scalar) const;

		Point& operator+=(Point other);
		Point& operator-=(Point other);
		Point& operator*=(float scalar);
		Point& operator/=(float scalar);

		bool operator==(Point other) const;
	};

}