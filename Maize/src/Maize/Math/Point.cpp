#include "Maize/Math/Point.h"

namespace Maize {

    const Point Point::one = Point(1, 1);
    const Point Point::zero = Point(0, 0);
    const Point Point::up = Point(0, -1);
    const Point Point::down = Point(0, 1);
    const Point Point::left = Point(-1, 0);
    const Point Point::right = Point(1, 0);

    Point::Point(int32_t x, int32_t y) : x(x), y(y) {}

    Point::Point(const sf::Vector2i& point) : x(point.x), y(point.y) {}

    void Point::Clamp(Point min, Point max)
    {
        x = std::max(std::min(x, max.x), min.x);
        y = std::max(std::min(y, max.y), min.y);
    }

    float Point::Magnitude() const
    {
        return std::sqrt(float(x * x + y * y));
    }

    float Point::Distance(Point a, Point b)
    {
        float dx = float(b.x - a.x);
        float dy = float(b.y - a.y);
        return std::sqrt(dx * dx + dy * dy);
    }

    Point Point::CeilToInt(Point other)
    {
        return Point(int(std::ceil(other.x)), int(std::ceil(other.y)));
    }

    Point Point::FloorToInt(Point other)
    {
        return Point(int(std::floor(other.x)), int(std::floor(other.y)));
    }

    Point Point::RoundToInt(Point other)
    {
        return Point(int(std::round(other.x)), int(std::round(other.y)));
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

    Point::operator sf::Vector2i() const
    {
        return sf::Vector2i(x, y);
    }

    Point::operator PointF() const
    {
        return PointF(float(x), float(y));
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
        return Point(int(x * scalar), int(y * scalar));
    }

    Point Point::operator/(float scalar) const
    {
        return Point(int(x / scalar), int(y / scalar));
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
        x = int(x * scalar);
        y = int(y * scalar);
        return *this;
    }

    Point& Point::operator/=(float scalar)
    {
        x = int(x / scalar);
        y = int(y / scalar);
        return *this;
    }

    bool Point::operator==(Point other) const
    {
        return x == other.x && y == other.y;
    }

}