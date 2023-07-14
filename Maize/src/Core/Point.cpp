#include "Core/Point.h"


namespace Maize {

    const Point Point::one = Point(1, 1);
    const Point Point::zero = Point(0, 0);
    const Point Point::up = Point(0, -1);
    const Point Point::down = Point(0, 1);
    const Point Point::left = Point(-1, 0);
    const Point Point::right = Point(1, 0);

    Point::Point(int32_t x, int32_t y) : x(x), y(y) {}

    Point::Point(SDL_Point point) : x(point.x), y(point.y) {}

    void Point::Clamp(Point min, Point max)
    {
        x = std::max(std::min(x, max.x), min.x);
        y = std::max(std::min(y, max.y), min.y);
    }

    float Point::Magnitude() const
    {
        return std::sqrt(floatCast(x * x + y * y));
    }

    float Point::Distance(Point a, Point b)
    {
        float dx = floatCast(b.x - a.x);
        float dy = floatCast(b.y - a.y);
        return std::sqrt(dx * dx + dy * dy);
    }

    Point Point::CeilToInt(Point other)
    {
        return Point(intCast(std::ceil(other.x)), intCast(std::ceil(other.y)));
    }

    Point Point::FloorToInt(Point other)
    {
        return Point(intCast(std::floor(other.x)), intCast(std::floor(other.y)));
    }

    Point Point::RoundToInt(Point other)
    {
        return Point(intCast(std::round(other.x)), intCast(std::round(other.y)));
    }

    Point Point::Max(Point other1, Point other2)
    {
        int32_t x = std::max(other1.x, other2.x);
        int32_t y = std::max(other1.y, other2.y);
        return Point(x, y);
    }

    Point Point::Min(Point other1, Point other2)
    {
        int32_t x = std::min(other1.x, other2.x);
        int32_t y = std::min(other1.y, other2.y);
        return Point(x, y);
    }

    Point::operator SDL_Point() const
    {
        return { x, y };
    }

    Point::operator const SDL_Point* () const
    {
        return reinterpret_cast<const SDL_Point*>(this);
    }

    Point::operator PointF() const
    {
        return PointF(floatCast(x), floatCast(y));
    }

    Point Point::operator+(Point other) const
    {
        return Point(x + other.x, y + other.y);
    }

    Point Point::operator-(Point other) const
    {
        return Point(x - other.x, y - other.y);
    }

    Point Point::operator*(float scalar) const
    {
        return Point(intCast(x * scalar), intCast(y * scalar));
    }

    Point Point::operator/(float scalar) const
    {
        return Point(intCast(x / scalar), intCast(y / scalar));
    }

    Point& Point::operator+=(Point other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    Point& Point::operator-=(Point other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    Point& Point::operator*=(float scalar)
    {
        x = intCast(x * scalar);
        y = intCast(y * scalar);
        return *this;
    }

    Point& Point::operator/=(float scalar)
    {
        x = intCast(x / scalar);
        y = intCast(y / scalar);
        return *this;
    }

    bool Point::operator==(Point other) const
    {
        return x == other.x && y == other.y;
    }

}