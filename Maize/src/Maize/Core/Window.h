#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <cstdint>
#include <string>

#include "Maize/Events/WindowEvents.h"
#include "Maize/Events/MouseEvents.h"
#include "Maize/Events/KeyEvents.h"
#include "Maize/Events/Event.h"
#include "Maize/Renderer/Colour.h"

namespace Maize {

    class Window
    {
    public:
        using EventCallBackFn = std::function<void(Event&)>;

        explicit Window(const std::string& title, uint32_t width = 1280, uint32_t height = 720);

        const std::string& GetTitle() const { return m_WindowData.title; }
        uint32_t GetWidth() const { return m_WindowData.width; }
        uint32_t GetHeight() const { return m_WindowData.height; }
        bool IsVSync() const { return m_WindowData.vSync; }

        void SetEventCallback(const EventCallBackFn& callback) { m_WindowData.eventCallback = callback; }
        void SetTitle(const std::string& title);
        void SetVSync(bool enable);
        void SetView(const sf::View& view) { m_Window.setView(view); }
        const sf::View& GetView() const { return m_Window.getView(); }

        void PollEvent();

        void Clear(Colour clearColour);
        void Render(const sf::Drawable& drawable);
        void Display();

        operator sf::RenderWindow&();

    private:
        sf::RenderWindow m_Window;

        struct WindowData
        {
            std::string title;
            uint32_t width = 0;
            uint32_t height = 0;
            bool vSync = false;
            EventCallBackFn eventCallback;
        };

        WindowData m_WindowData;
    };

} // Maize