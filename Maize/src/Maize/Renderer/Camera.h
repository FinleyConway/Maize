#pragma once

#include <SFML/Graphics/View.hpp>

#include "Maize/Core/Application.h"
#include "Maize/Events/Event.h"

namespace Maize {

    class Camera
    {
    public:
        Camera()
        {
            auto& app = Application::Get();
            const sf::RenderWindow& window = app.GetWindow();

            m_Camera = window.getDefaultView();
            m_Camera.setCenter(0, 0);
        }

        static sf::Vector2f ScreenToWorld(sf::Vector2i screenPosition)
        {
            const sf::RenderWindow& window = Application::Get().GetWindow();
            return window.mapPixelToCoords(screenPosition);
        }

        static sf::Vector2i WorldToScreen(sf::Vector2f worldPosition)
        {
            const sf::RenderWindow& window = Application::Get().GetWindow();
            return window.mapCoordsToPixel(worldPosition);
        }

    protected:
        sf::View m_Camera;
    };

} // Maize