#pragma once

#include "Maize/Core/Application.h"
#include "Maize/Events/Event.h"

namespace Maize {

    class Camera
    {
    public:
        Camera()
        {
            auto& ren = Application::Get().GetRenderer();

            m_Camera = ren.GetCurrentTexture()->getDefaultView();
            m_Camera.setCenter(0, 0);
        }

        static sf::Vector2f ScreenToWorld(sf::Vector2i screenPosition)
        {
            auto& ren = Application::Get().GetRenderer();
            auto& window = Application::Get().GetWindow().GetRenderWindow();

            return window.mapPixelToCoords(screenPosition, ren.GetCurrentTexture()->getView());
        }

        static sf::Vector2i WorldToScreen(sf::Vector2f worldPosition)
        {
            auto& ren = Application::Get().GetRenderer();
            auto& window = Application::Get().GetWindow().GetRenderWindow();

            return window.mapCoordsToPixel(worldPosition, ren.GetCurrentTexture()->getView());
        }

    protected:
        sf::View m_Camera;
    };

} // Maize