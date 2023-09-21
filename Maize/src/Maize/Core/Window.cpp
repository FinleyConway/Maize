#include "Maize/Core/Window.h"
#include <imgui-SFML.h>

namespace Maize {

    Window::Window(const std::string& title, sf::Vector2u windowSize) : m_WindowSize(windowSize)
    {
        Create();
        SetTitle(title);
        SetVSync(true);
    }

    Window::~Window()
    {
        m_Window.close();
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
        sf::Event e;

        while (m_Window.pollEvent(e))
        {
            ImGui::SFML::ProcessEvent(m_Window, e);

            if (e.type == sf::Event::Closed)
            {
                WindowCloseEvent event;
                m_EventCallback(event);
            }
            else if (e.type == sf::Event::Resized)
            {
                m_WindowSize.x = e.size.width;
                m_WindowSize.y = e.size.height;

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
        m_Window.close();
        m_IsFullscreen = !m_IsFullscreen;
        Create();
        m_WindowSize = m_Window.getSize();
    }

    sf::FloatRect Window::GetViewSpace() const
    {
        sf::Vector2f viewCenter(m_Window.getView().getCenter());
        sf::Vector2f viewSize(m_Window.getView().getSize());
        sf::Vector2f viewSizeHalf(viewSize.x / 2, viewSize.y / 2);
        sf::FloatRect viewSpace(viewCenter - viewSizeHalf, viewSize);

        return viewSpace;
    }

    void Window::BeginDrawing(Renderer& renderer, sf::Color clearColour)
    {
        m_Window.clear(clearColour);
        renderer.BeginDrawing();
    }

    void Window::EndDrawing(Renderer& renderer)
    {
        m_Window.display();
        renderer.EndDrawing();
    }

    void Window::Create()
    {
        uint32_t style = m_IsFullscreen ? sf::Style::Fullscreen : sf::Style::Default;
        m_Window.create(sf::VideoMode(m_WindowSize.x, m_WindowSize.y), m_WindowTitle, style);
    }

} // Maize