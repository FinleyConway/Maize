#pragma once

#include "Maize/Renderer/Camera.h"
#include "Maize/Core/Input.h"

namespace Maize {

    class EditorCamera : public Camera
    {
    public:
        void OnEvent(Event& e)
        {
            EventDispatcher dispatcher(e);
            dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&EditorCamera::OnMouseScroll, this, std::placeholders::_1));
            dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&EditorCamera::OnMouseButtonPressed, this, std::placeholders::_1));
            dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&EditorCamera::OnMouseButtonReleased, this, std::placeholders::_1));

            dispatcher.Dispatch<WindowResizeEvent>(std::bind(&EditorCamera::OnWindowResizeEvent, this, std::placeholders::_1));
        }

        void OnUpdate(float deltaTime)
        {
            if (m_MiddleMouseButton)
            {
                PanMouse();
            }

            UpdateView();
        }

    private:
        void UpdateView()
        {
            auto& window = Application::Get().GetWindow();

            //window.SetView(m_Camera); // causing issues with frame buffer with tile placement
        }

        void PanMouse()
        {
            sf::Vector2f currentMousePos = Camera::ScreenToWorld(Input::GetMousePosition());
            sf::Vector2f displacement = m_InitialMousePosition - currentMousePos;
            m_Camera.move(displacement);
        }

        void CameraZoom(int32_t offset)
        {
            float factor = 1.0f - offset * 0.1f;
            m_Camera.zoom(factor);
        }

        bool OnMouseScroll(const MouseScrolledEvent& e)
        {
            CameraZoom(e.GetYOffset());
            UpdateView();

            return false;
        }

        bool OnMouseButtonPressed(const MouseButtonPressedEvent& e)
        {
            if (e.GetMouseButton() == MouseCode::Middle)
            {
                m_MiddleMouseButton = true;
                m_InitialMousePosition = Camera::ScreenToWorld(Input::GetMousePosition());
                return true;
            }

            return false;
        }

        bool OnMouseButtonReleased(const MouseButtonReleasedEvent& e)
        {
            if (e.GetMouseButton() == MouseCode::Middle)
            {
                m_MiddleMouseButton = false;
                return true;
            }

            return false;
        }

        bool OnWindowResizeEvent(const WindowResizeEvent& e)
        {
            m_Camera.setSize((float)e.GetWidth(), (float)e.GetHeight());
            return false;
        }

        bool m_MiddleMouseButton = false;
        sf::Vector2f m_InitialMousePosition;
    };

} // Maize