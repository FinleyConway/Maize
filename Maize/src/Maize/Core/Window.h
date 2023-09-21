#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include <cstdint>
#include <string>

#include "Maize/Events/Event.h"
#include "Maize/Renderer/Renderer.h"

#include "Maize/Events/WindowEvents.h"
#include "Maize/Events/MouseEvents.h"
#include "Maize/Events/KeyEvents.h"

namespace Maize {

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        explicit Window(const std::string& title, sf::Vector2u windowSize = { 1280, 720 });
        ~Window();

        void SetTitle(const std::string& title);
        const std::string& GetTitle() const { return m_WindowTitle; }

        uint32_t GetWidth() const { return m_WindowSize.x; }
        uint32_t GetHeight() const { return m_WindowSize.y; }
        float GetAspectRatio() const { return (float)m_WindowSize.x / (float)m_WindowSize.y; }

        void SetVSync(bool enable);
        bool IsVSyncEnabled() const { return m_IsVSyncEnabled; }

        void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }
        void SetView(const sf::View& view) { m_Window.setView(view); }

        void PollEvents();

        void ToggleFullscreen();

        sf::FloatRect GetViewSpace() const;

        void BeginDrawing(Renderer& renderer, sf::Color clearColour = sf::Color::Black);
        void EndDrawing(Renderer& renderer);

        sf::RenderWindow& GetRenderWindow() { return m_Window; }

    private:
        void Create();

    private:
        sf::RenderWindow m_Window;

        sf::Vector2u m_WindowSize;
        std::string m_WindowTitle;
        bool m_IsFullscreen = false;
        bool m_IsVSyncEnabled = false;
        EventCallbackFn m_EventCallback;
    };

} // Maize