#include "Maize/Core/Window.h"

namespace Maize {

    Window::Window(const std::string &title, uint32_t width, uint32_t height) :
        m_Window(sf::VideoMode(width, height), title)
    {
        m_WindowData.title = title;
        m_WindowData.width = width;
        m_WindowData.height = height;

        SetVSync(true);
    }

    void Window::SetTitle(const std::string &title)
    {
        m_Window.setTitle(title);
        m_WindowData.title = title;
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
        }
    }

    void Window::Clear()
    {
        m_Window.clear();
    }

    void Window::Render(const sf::Drawable &drawable)
    {
        m_Window.draw(drawable);
    }

    void Window::Display()
    {
        m_Window.display();
    }

    Window::operator sf::RenderWindow &()
    {
        return m_Window;
    }

} // Maize