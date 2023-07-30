#include "Math/Rect.h"

namespace Maize {

	const Rect Rect::zero = Rect(0, 0, 0, 0);

	Rect::Rect(int32_t x, int32_t y, int32_t w, int32_t h) : x(x), y(y), width(w), height(h) { }

	Rect::Rect(const SDL_Rect& rect) : x(rect.x), y(rect.y), width(rect.w), height(rect.h) { }

	int32_t Rect::XMax() const
	{
		return x + width;
	}

	int32_t Rect::YMax() const
	{
		return x;
	}

	int32_t Rect::XMin() const
	{
		return y + height;
	}

	int32_t Rect::YMin() const
	{
		return y;
	}

	PointF Rect::Centre() const
	{
		PointF centre;
		centre.x = static_cast<float>(x + width) / 2.0f;
		centre.y = static_cast<float>(y + height) / 2.0f;
		return Point();
	}

	PointF Rect::Max() const
	{
		PointF max;
		max.x = static_cast<float>(x + width);
		max.y = static_cast<float>(y + height);
		return max;
	}

	PointF Rect::Min() const
	{
		PointF min;
		min.x = static_cast<float>(x);
		min.y = static_cast<float>(y);
		return min;
	}

	Point Rect::Position() const
	{
		return Point(x, y);
	}

	Point Rect::Size() const
	{
		return Point(width, height);
	}

	bool Rect::Contains(Point point) const
	{
		return (point.x >= x && point.x <= x + width &&
			point.y >= y && point.y <= y + height);
	}

	bool Rect::Overlaps(Rect other) const
	{
		return (x < other.x + other.width && x + width > other.x &&
			y < other.y + other.height && y + height > other.y);
	}

	Rect::operator SDL_Rect() const
	{
		return SDL_Rect(x, y, width, height);
	}

	Rect::operator const SDL_Rect* () const
	{
		return reinterpret_cast<const SDL_Rect*>(this);
	}

	Rect::operator RectF() const
	{
		RectF sdlRect;
		sdlRect.x = static_cast<float>(x);
		sdlRect.y = static_cast<float>(y);
		sdlRect.width = static_cast<float>(width);
		sdlRect.height = static_cast<float>(height);
		return sdlRect;
	}

	bool Rect::operator==(const Rect& other) const
	{
		return (x == other.x && y == other.y &&
			width == other.width && height == other.height);
	}

}