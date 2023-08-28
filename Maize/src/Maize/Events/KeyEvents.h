#pragma once

#include "Maize/Events/Event.h"
#include "Maize/Core/KeyCodes.h"

namespace Maize {

    class KeyEvent : public Event
    {
    public:
        KeyCode GetKeyCode() const { return m_KeyCode; }

    protected:
        explicit KeyEvent(const KeyCode keycode) : m_KeyCode(keycode) { }

        KeyCode m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        explicit KeyPressedEvent(const KeyCode keycode) : KeyEvent(keycode) { }

        EVENT_CLASS_TYPE(KeyPressed)
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) { }

        EVENT_CLASS_TYPE(KeyReleased)
    };

} // Maize