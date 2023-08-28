#include "Maize/Math/RectF.h"

namespace Maize {

    const RectF RectF::zero = RectF(0, 0, 0, 0);

    RectF::RectF(float x, float y, float w, float h) : x(x), y(y), width(w), height(h) { }

    RectF::RectF(const sf::FloatRect& rect) : x(rect.getSize().x), y(rect.getSize().y), width(rect.width), height(rect.height) { }

    float RectF::XMax() const
    {
        return x + width;
    }

    float RectF::YMax() const
    {
        return y + height;
    }

    float RectF::XMin() const
    {
        return x;
    }

    float RectF::YMin() const
    {
        return y;
    }

    PointF RectF::Centre() const
    {
        PointF center;
        center.x = x + width / 2.0f;
        center.y = y + height / 2.0f;
        return center;
    }

    PointF RectF::Max() const
    {
        PointF maxPoint;
        maxPoint.x = x + width;
        maxPoint.y = y + height;
        return maxPoint;
    }

    PointF RectF::Min() const
    {
        PointF minPoint;
        minPoint.x = x;
        minPoint.y = y;
        return minPoint;
    }

    PointF RectF::Position() const
    {
        return PointF(x, y);
    }

    PointF RectF::Size() const
    {
        return PointF(width, height);
    }

    bool RectF::Contains(PointF point) const
    {
        return (point.x >= x && point.x <= x + width &&
                point.y >= y && point.y <= y + height);
    }

    bool RectF::Overlaps(RectF other) const
    {
        return (x < other.x + other.width && x + width > other.x &&
                y < other.y + other.height && y + height > other.y);
    }

    RectF::operator sf::FloatRect () const
    {
        return sf::FloatRect(x, y, width, height);
    }

    bool RectF::operator==(const RectF& other) const
    {
        return (x == other.x && y == other.y &&
                width == other.width && height == other.height);
    }

} // Maize