#pragma once

#include "Maize/Core/Window.h"
#include "Maize/Renderer/Renderer.h"

#include "Maize/Core/LayerStack.h"

int main();

namespace Maize {

	class Layer;
	class InputLayer;
	class GameLayer;
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
		~Application() = default;

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);

    private:
        friend int ::main();

        void Run();

        void OnEvent(Event& e);
        bool OnWindowClose(const WindowCloseEvent& e);
		bool OnWindowResize(const WindowResizeEvent& e);

    private:
        Window m_Window;
		Renderer m_Renderer;

        LayerStack m_LayerStack;
		InputLayer* m_InputLayer = nullptr;
		GameLayer* m_GameLayer = nullptr;

        bool m_IsRunning = true;
    };

    // to be used init maize
    std::unique_ptr<Application> CreateApplication();

} // Maize