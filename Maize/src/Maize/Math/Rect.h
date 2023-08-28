#pragma once

#include <SFML/Graphics/Rect.hpp>

#include "Maize/Math/Point.h"
#include "Maize/Math/RectF.h"

namespace Maize {

    struct Rect
    {
    public:
        Rect() = default;
        Rect(int32_t x, int32_t y, int32_t w, int32_t h);
        Rect(const sf::IntRect& rect);

        int32_t x = 0;
        int32_t y = 0;
        int32_t width = 0;
        int32_t height = 0;

        const static Rect zero;

        int32_t XMax() const;
        int32_t YMax() const;
        int32_t XMin() const;
        int32_t YMin() const;

        PointF Centre() const;
        PointF Max() const;
        PointF Min() const;

        Point Position() const;
        Point Size() const;

        bool Contains(Point point) const;
        bool Overlaps(Rect other) const;

        operator sf::IntRect () const;
        operator RectF () const;

        bool operator==(const Rect& other) const;
    };

} // Maize