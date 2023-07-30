#pragma once

#include <SDL.h>

#include "Point.h"

namespace Maize {

	struct RectF
	{
	public:
		RectF() = default;
		RectF(float x, float y, float w, float h);
		RectF(const SDL_FRect& rect);

		float x = 0;
		float y = 0;
		float width = 0;
		float height = 0;

		const static RectF zero;

		float XMax() const;
		float YMax() const;
		float XMin() const;
		float YMin() const;

		PointF Centre() const;
		PointF Max() const;
		PointF Min() const;

		PointF Position() const;
		PointF Size() const;

		bool Contains(PointF point) const;
		bool Overlaps(RectF other) const;

		operator SDL_FRect() const;

		bool operator==(const RectF& other) const;
	};

}