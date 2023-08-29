#pragma once

#include <cstdint>

#include "Maize/Events/Event.h"
#include "Maize/Core/KeyCodes.h"

namespace Maize {

    class MouseButtonEvent : public Event
    {
    public:
        MouseCode GetMouseButton() const { return m_Button; }

    protected:
        explicit MouseButtonEvent(const MouseCode button) : m_Button(button) { }

        MouseCode m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) { }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) { }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const int32_t x, const int32_t y) : m_MouseX(x), m_MouseY(y) { }

        int32_t GetX() const { return m_MouseX; }
        int32_t GetY() const { return m_MouseY; }

        EVENT_CLASS_TYPE(MouseMoved)

    private:
        int32_t m_MouseX;
        int32_t m_MouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const int32_t xOffset, const int32_t yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

        int32_t GetXOffset() const { return m_XOffset; }
        int32_t GetYOffset() const { return m_YOffset; }

        EVENT_CLASS_TYPE(MouseScrolled)

    private:
        int32_t m_XOffset;
        int32_t m_YOffset;
    };

} // Maize