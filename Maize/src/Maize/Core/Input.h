#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Maize/Core/KeyCodes.h"

namespace Maize {

    class Input
    {
    public:
        static bool IsKeyPressed(const KeyCode key)
        {
            return sf::Keyboard::isKeyPressed(static_cast<sf::Keyboard::Key>(key));
        }

        static bool IsMouseButtonPressed(const MouseCode button)
        {
            return sf::Mouse::isButtonPressed(static_cast<sf::Mouse::Button>(button));
        }

        static sf::Vector2i GetMousePosition()
        {
            return sf::Mouse::getPosition(Application::Get().GetWindow());
        }
    };

} // Maize