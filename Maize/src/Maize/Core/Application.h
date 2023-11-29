#pragma once

#include "Maize/Core/Window.h"
#include "Maize/Core/LayerStack.h"
#include "Maize/Events/EventDispatcher.h"
#include "Maize/ImGui/ImGuiLayer.h"

int main();

namespace Maize {

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
        ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

        void Close() { m_IsRunning = false; }

    private:
        friend int ::main();

        void Run();
        void OnEvent(Event& e);

        bool OnWindowClose(const WindowCloseEvent& e);
		bool OnWindowResize(const WindowResizeEvent& e);

    private:
        inline static Application* s_Instance = nullptr;

        Window m_Window;

        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;

        bool m_IsRunning = true;
        bool m_Minimized = false;
    };

    // to be used init maize
    std::unique_ptr<Application> CreateApplication();

} // Maize