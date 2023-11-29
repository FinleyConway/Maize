#include "mpch.h"
#include "Maize/Core/Application.h"

namespace Maize {

    Application::Application(const ApplicationSpecification &specification) : m_Window(specification.name)
    {
        s_Instance = this;

        m_Window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

		Renderer::Initialize(m_Window.GetSize());
    }

	Application::~Application()
	{
		Renderer::Shutdown();
	}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
        {
            if (e.handled) break;
            (*it)->OnEvent(e);
        }
    }

    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer *layer)
    {
        m_LayerStack.PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::Run()
    {
        sf::Clock clock;

        float lastFrameTime = 0.0f;
        float deltaTime = 0.0f;

        while (m_IsRunning)
        {
            float currentTime = sf::Time(clock.getElapsedTime()).asSeconds();
            deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;

            m_Window.PollEvents();

            if (!m_Minimized)
            {
                m_ImGuiLayer->Begin(m_Window, deltaTime);

                for (Layer* layer : m_LayerStack)
                    layer->OnUpdate(deltaTime);

                m_Window.BeginDrawing();

                Renderer::DrawBufferTexture(); // temp
                m_ImGuiLayer->End(m_Window);

                m_Window.EndDrawing();
            }
        }
    }

    bool Application::OnWindowClose(const WindowCloseEvent &e)
    {
        Close();
        return false;
    }

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		Renderer::OnWindowResize(sf::Vector2u(e.GetWidth(), e.GetHeight()));
		return false;
	}

} // Maize