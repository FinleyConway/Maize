#pragma once

#include <SDL.h>

namespace Maize {

    enum class MouseCode
    {
        // main buttons
        Left = SDL_BUTTON_LEFT,
        Middle = SDL_BUTTON_MIDDLE,
        Right = SDL_BUTTON_RIGHT,

        // side buttons
        Alt1 = SDL_BUTTON_X1,
        Alt2 = SDL_BUTTON_X2,
    };

}