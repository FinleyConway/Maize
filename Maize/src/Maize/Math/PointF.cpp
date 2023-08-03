#include "Maize/Math/PointF.h"

namespace Maize {

    const PointF PointF::one = PointF(1.0f, 1.0f);
    const PointF PointF::zero = PointF(0.0f, 0.0f);
    const PointF PointF::up = PointF(0.0f, -1.0f);
    const PointF PointF::down = PointF(0.0f, 1.0f);
    const PointF PointF::left = PointF(-1.0f, 0.0f);
    const PointF PointF::right = PointF(1.0f, 0.0f);

    PointF::PointF(float x, float y) : x(x), y(y) {}

    PointF::PointF(SDL_FPoint point) : x(point.x), y(point.y) {}

    float PointF::Angle(PointF from, PointF to)
    {
        float dot = Dot(from.Normalized(), to.Normalized());
        return std::acos(std::fmaxf(std::fminf(dot, 1.0f), -1.0f)) * (180.0f / 3.1415926535f);
    }

    float PointF::Distance(PointF a, PointF b)
    {
        float dx = b.x - a.x;
        float dy = b.y - a.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float PointF::Dot(PointF other1, PointF other2)
    {
        return other1.x * other2.x + other1.y * other2.y;
    }

    PointF PointF::ClampMagnitude(PointF other, float maxLength)
    {
        float sqrMagnitude = other.x * other.x + other.y * other.y;
        if (sqrMagnitude > maxLength * maxLength)
        {
            float magnitude = std::sqrt(sqrMagnitude);
            float normalizedX = other.x / magnitude;
            float normalizedY = other.y / magnitude;
            return PointF(normalizedX * maxLength, normalizedY * maxLength);
        }
        return other;
    }

    PointF PointF::Lerp(PointF a, PointF b, float t)
    {
        t = std::fmaxf(std::fminf(t, 1.0f), 0.0f);
        float x = a.x + (b.x - a.x) * t;
        float y = a.y + (b.y - a.y) * t;
        return PointF(x, y);
    }

    PointF PointF::Max(PointF other1, PointF other2)
    {
        float x = std::fmaxf(other1.x, other2.x);
        float y = std::fmaxf(other1.y, other2.y);
        return PointF(x, y);
    }

    PointF PointF::Min(PointF other1, PointF other2)
    {
        float x = std::fminf(other1.x, other2.x);
        float y = std::fminf(other1.y, other2.y);
        return PointF(x, y);
    }

    PointF PointF::Reflect(PointF inDirection, PointF inNormal)
    {
        float dot = Dot(inDirection, inNormal);
        float x = inDirection.x - 2.0f * dot * inNormal.x;
        float y = inDirection.y - 2.0f * dot * inNormal.y;
        return PointF(x, y);
    }

    float PointF::Magnitude() const
    {
        return std::sqrt(x * x + y * y);
    }

    PointF PointF::Normalized() const
    {
        float magnitude = Magnitude();
        return PointF(x / magnitude, y / magnitude);
    }

    PointF::operator SDL_FPoint() const
    {
        return { x, y };
    }

    PointF::operator const SDL_FPoint* () const
    {
        return reinterpret_cast<const SDL_FPoint*>(this);
    }

    PointF::operator const SDL_Point* () const
    {
        return reinterpret_cast<const SDL_Point*>(this);
    }

    PointF PointF::operator+(PointF other) const
    {
        return PointF(x + other.x, y + other.y);
    }

    PointF PointF::operator-(PointF other) const
    {
        return PointF(x - other.x, y - other.y);
    }

    PointF PointF::operator*(float scalar) const
    {
        return PointF(x * scalar, y * scalar);
    }

    PointF PointF::operator/(float scalar) const
    {
        return PointF(x / scalar, y / scalar);
    }

    PointF& PointF::operator+=(PointF other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    PointF& PointF::operator-=(PointF other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    PointF& PointF::operator*=(float scalar)
    {
        x *= scalar;
        y *= scalar;
        return *this;
    }

    PointF& PointF::operator/=(float scalar)
    {
        x /= scalar;
        y /= scalar;
        return *this;
    }

    bool PointF::operator==(PointF other) const
    {
        float dx = std::abs(x - other.x);
        float dy = std::abs(y - other.y);

        return dx < 1e-5 && dy < 1e-5;
    }

}