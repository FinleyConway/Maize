#pragma once

#include "Maize/Core/Window.h"
#include "Maize/Core/LayerStack.h"

int main();

namespace Maize {

	class Layer;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;

    struct ApplicationSpecification
    {
        std::string name = "Maize";
    };

    class Application
    {
    public:
        explicit Application(const ApplicationSpecification& specification);
		~Application();

        static Application& Get() { return *s_Instance; }

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

        Window& GetWindow() { return m_Window; }

    private:
        friend int ::main();

        void Run();
        void OnEvent(Event& e);

        bool OnWindowClose(const WindowCloseEvent& e);

    private:
        inline static Application* s_Instance = nullptr;

        Window m_Window;
        LayerStack m_LayerStack;
        bool m_IsRunning = true;
    };

    // to be used init maize
    std::unique_ptr<Application> CreateApplication();

} // Maize