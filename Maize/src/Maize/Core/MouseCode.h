#pragma once

#include <SFML/Window.hpp>

namespace Maize {

    enum class MouseCode
    {
        // main buttons
        Left = sf::Mouse::Left,
        Middle = sf::Mouse::Middle,
        Right = sf::Mouse::Right,

        // side buttons
        Alt1 = sf::Mouse::XButton1,
        Alt2 = sf::Mouse::XButton2,
    };

}