#pragma once

#include "Maize/Core/Application.h"
#include "Maize/Events/Event.h"

namespace Maize {

    class Camera
    {
    public:
        Camera()
        {
            m_Camera = Renderer::GetCurrentTexture()->getDefaultView();
            m_Camera.setCenter(0, 0);
        }

        static sf::Vector2f ScreenToWorld(sf::Vector2i screenPosition)
        {
            auto& window = Application::Get().GetWindow().GetRenderWindow();

            return window.mapPixelToCoords(screenPosition, Renderer::GetCurrentTexture()->getView());
        }

        static sf::Vector2i WorldToScreen(sf::Vector2f worldPosition)
        {
            auto& window = Application::Get().GetWindow().GetRenderWindow();

            return window.mapCoordsToPixel(worldPosition, Renderer::GetCurrentTexture()->getView());
        }

    protected:
        sf::View m_Camera;
    };

} // Maize