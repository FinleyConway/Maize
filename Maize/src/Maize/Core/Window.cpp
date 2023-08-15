#include "Maize/Core/Window.h"

#include <imgui-SFML.h>

namespace Maize {

	Window::Window(const std::string& title, Point windowSize) :
		m_Window(sf::VideoMode(windowSize.x, windowSize.y), title)
	{
		m_WindowData.title = title;
		m_WindowData.width = windowSize.x;
		m_WindowData.height = windowSize.y;

		SetVSync(true);
	}

	void Window::SetVSync(bool enable)
	{
		m_Window.setVerticalSyncEnabled(enable);
		m_WindowData.vSync = enable;
	}

	void Window::PollEvent()
	{
		sf::Event e;

		while (m_Window.pollEvent(e))
		{
			ImGui::SFML::ProcessEvent(m_Window, e);

			if (e.type == sf::Event::Closed)
			{
				WindowCloseEvent event;
				m_WindowData.eventCallback(event);
			}
			else if (e.type == sf::Event::Resized)
			{
				m_WindowData.width = e.size.width;
				m_WindowData.height = e.size.height;

				WindowResizeEvent event(e.size.width, e.size.height);
				m_WindowData.eventCallback(event);
			}
			else if (e.type == sf::Event::KeyPressed)
			{
				KeyPressedEvent event((KeyCode)e.key.code);
				m_WindowData.eventCallback(event);
			}
			else if (e.type == sf::Event::KeyReleased)
			{
				KeyReleasedEvent event((KeyCode)e.key.code);
				m_WindowData.eventCallback(event);
			}
			else if (e.type == sf::Event::MouseButtonPressed)
			{
				MouseButtonPressedEvent event((MouseCode)e.mouseButton.button);
				m_WindowData.eventCallback(event);
			}
			else if (e.type == sf::Event::MouseButtonReleased)
			{
				MouseButtonReleasedEvent event((MouseCode)e.mouseButton.button);
				m_WindowData.eventCallback(event);
			}
			else if (e.type == sf::Event::MouseMoved)
			{
				MouseMovedEvent event(e.mouseMove.x, e.mouseMove.y);

				std::cout << e.mouseMove.x << " " << e.mouseMove.y <<  std::endl;

				m_WindowData.eventCallback(event);
			}
			else if (e.type == sf::Event::MouseWheelScrolled)
			{
				MouseScrolledEvent event(e.mouseWheelScroll.x, e.mouseWheelScroll.y);
				m_WindowData.eventCallback(event);
			}
		}
	}

	void Window::Clear(Colour colour)
	{
		m_Window.clear(colour);
	}

	void Window::Render(const sf::Drawable& drawable)
	{
		m_Window.draw(drawable, sf::RenderStates::Default);
	}

	void Window::Present()
	{
		m_Window.display();
	}

	void Window::SetTitle(const std::string& title)
	{
		m_WindowData.title = title;
		m_Window.setTitle(title);
	}

	Window::operator sf::RenderWindow& ()
	{
		return m_Window;
	}

}