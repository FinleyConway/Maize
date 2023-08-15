#include "Maize/Core/Application.h"

namespace Maize {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification) :
		m_Window(specification.name)
	{
		s_Instance = this;

		m_Window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			(*it)->OnEvent(e);
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::Run()
	{
		sf::Clock clock;

		float lastFrameTime = sf::Time(clock.getElapsedTime()).asSeconds();
		float deltaTime = 0.0f;

		while (m_Running)
		{
			float currentTime = sf::Time(clock.getElapsedTime()).asSeconds();
			deltaTime = currentTime - lastFrameTime;
			lastFrameTime = currentTime;

			if (!m_Minimized)
			{
				m_Window.PollEvent();

				m_ImGuiLayer->Begin(clock.restart());

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);

				m_Window.Clear(Colour::grey);

				for (Layer* layer : m_LayerStack)
					layer->OnRender();

				m_ImGuiLayer->End();

				m_Window.Present();
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}