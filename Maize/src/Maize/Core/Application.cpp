#include "mpch.h"
#include "Maize/Core/Application.h"

#include "Maize/Renderer/Renderer.h"

#include "Maize/Events/EventDispatcher.h"
#include "Maize/Events/WindowEvents.h"

#include "Maize/Core/Layer.h"

#include "Maize/Scene/SceneManager.h"

namespace Maize {

    Application::Application(const ApplicationSpecification &specification) : m_Window(specification.name)
    {
        s_Instance = this;

        m_Window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Renderer::Initialize(m_Window.GetRenderWindow());

		m_LayerStack.PushLayer(m_SceneManager = new SceneManager());
    }

	Application::~Application()
	{
	}

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
        dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Application::OnWindowResize, this, std::placeholders::_1));

		m_Input.OnEvent(e);

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

			Renderer::BeginDrawing(sf::Color::Black);

			for (Layer* layer : m_LayerStack)
				layer->OnUpdate(deltaTime);

			Renderer::EndDrawing();
        }
    }

    bool Application::OnWindowClose(const WindowCloseEvent &e)
    {
       	m_IsRunning = false;
        return false;
    }

	bool Application::OnWindowResize(const WindowResizeEvent& e)
	{
		Renderer::OnWindowResize(sf::Vector2f(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight())));
		return false;
	}

} // Maize