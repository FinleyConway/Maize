#include "mpch.h"
#include "Maize/Core/Window.h"

#include "Maize/Events/WindowEvents.h"
#include "Maize/Events/MouseEvents.h"
#include "Maize/Events/KeyEvents.h"

namespace Maize {

    Window::Window(const std::string& title, sf::Vector2u windowSize)
    {
        Create(windowSize);
        SetTitle(title);
        SetVSync(true);
    }

    void Window::SetTitle(const std::string& title)
    {
        m_Window.setTitle(title);
        m_WindowTitle = title;
    }

    void Window::SetVSync(bool enable)
    {
        m_Window.setVerticalSyncEnabled(enable);
        m_IsVSyncEnabled = enable;
    }

    void Window::PollEvents()
    {
        sf::Event e{};

        while (m_Window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                WindowCloseEvent event;
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::Resized)
            {
                WindowResizeEvent event(e.size.width, e.size.height);
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::KeyPressed)
            {
                KeyPressedEvent event((KeyCode)e.key.code);
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::KeyReleased)
            {
                KeyReleasedEvent event((KeyCode)e.key.code);
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::MouseButtonPressed)
            {
                MouseButtonPressedEvent event((MouseCode)e.mouseButton.button);
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::MouseButtonReleased)
            {
                MouseButtonReleasedEvent event((MouseCode)e.mouseButton.button);
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::MouseMoved)
            {
                MouseMovedEvent event(e.mouseMove.x, e.mouseMove.y);
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::MouseWheelMoved)
            {
                MouseScrolledEvent event(0, e.mouseWheel.delta);
                m_EventCallback(event);
            }
        }
    }

    void Window::ToggleFullscreen()
    {
        m_IsFullscreen = !m_IsFullscreen;
        Create({ 1280, 720 });
    }

	void Window::Create(sf::Vector2u windowSize)
	{
		if (m_IsFullscreen)
		{
			auto videoMode = sf::VideoMode::getDesktopMode();
			m_Window.create(videoMode, m_WindowTitle, sf::Style::Fullscreen);
		}
		else
		{
			m_Window.create(sf::VideoMode(windowSize.x, windowSize.y), m_WindowTitle, sf::Style::Close | sf::Style::Resize);
		}
	}

} // Maize