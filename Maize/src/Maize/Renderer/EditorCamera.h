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
            dispatcher.Dispatch<WindowResizeEvent>(std::bind(&EditorCamera::OnWindowResizeEvent, this, std::placeholders::_1));
        }

        void OnUpdate(float deltaTime)
        {
            // Camera movement using WASD keys
            float speed = 100.0f; // Adjust the movement speed as needed

            /*if (Input::IsKeyPressed(KeyCode::W))
                m_Camera.move(0.0f, -speed * deltaTime);
            if (Input::IsKeyPressed(KeyCode::A))
                m_Camera.move(-speed * deltaTime, 0.0f);
            if (Input::IsKeyPressed(KeyCode::S))
                m_Camera.move(0.0f, speed * deltaTime);
            if (Input::IsKeyPressed(KeyCode::D))
                m_Camera.move(speed * deltaTime, 0.0f);*/

            UpdateView();
        }

		void SetPosition(sf::Vector2f position)
		{
			m_Camera.setCenter(position);
			UpdateView();
		}

		void SetRotation(float angle)
		{
			m_Camera.setRotation(angle);
			UpdateView();
		}

    private:
        void UpdateView()
        {
			Renderer::GetCurrentTexture()->setView(m_Camera);
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

        bool OnWindowResizeEvent(const WindowResizeEvent& e)
        {
            m_Camera.setSize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
            return false;
        }
    };

} // Maize