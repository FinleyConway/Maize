#include "mpch.h"
#include "Maize/Core/Application.h"

#include "Maize/Events/EventDispatcher.h"
#include "Maize/Events/WindowEvents.h"

#include "Maize/Core/Layer.h"
#include "Maize/Layers/InputLayer.h"
#include "Maize/Layers/GameLayer.h"

namespace Maize {

    Application::Application(const ApplicationSpecification &specification) : m_Window(specification.name)
    {
        m_Window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
		m_Window.InitializeRenderer(m_Renderer);

		m_LayerStack.PushLayer(m_InputLayer = new InputLayer(m_Input));
		m_LayerStack.PushLayer(m_GameLayer = new GameLayer(m_Window, m_Renderer, m_Input));
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
        float lastFrameTime;
        float deltaTime;

        while (m_IsRunning)
        {
            float currentTime = sf::Time(clock.getElapsedTime()).asSeconds();
            deltaTime = currentTime - lastFrameTime;
            lastFrameTime = currentTime;

            m_Window.PollEvents();

			m_Renderer.BeginDrawing();

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);

			m_Renderer.EndDrawing();
        }
    }

    bool Application::OnWindowClose(const WindowCloseEvent &e)
    {
       	m_IsRunning = false;
        return false;
    }

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		m_Renderer.OnWindowResize(sf::Vector2f(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight())));
		return false;
	}

} // Maize