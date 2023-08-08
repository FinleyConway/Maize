#include "Maize/Core/Application.h"

namespace Maize {

	Application* Application::s_Instance = nullptr;

	Application::Application(const ApplicationSpecification& specification) :
		m_Window(specification.name),
		m_Renderer(m_Window)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return;
		if (~IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) return;
		if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) return;
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return;

		s_Instance = this;

		m_Window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImGuiLayer = new ImGuiLayer();
		m_Input = new InputPollingLayer();
		PushOverlay(m_ImGuiLayer);
		PushLayer(m_Input);
	}

	Application::~Application()
	{
		SDL_CloseAudio();
		Mix_Quit();
		IMG_Quit();
		SDL_Quit();
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
		float lastFrameTime = SDL_GetTicks() / 1000.0f;
		float deltaTime = 0.0f;

		while (m_Running)
		{
			float currentTime = SDL_GetTicks() / 1000.0f;
			deltaTime = currentTime - lastFrameTime;
			lastFrameTime = currentTime;

			if (!m_Minimized)
			{
				m_Window.PollEvent();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(deltaTime);

				m_ImGuiLayer->Begin();

				m_Renderer.Clear();

				for (Layer* layer : m_LayerStack)
					layer->OnRender();

				m_ImGuiLayer->End();
				m_Input->End();

				m_Renderer.Present();
			}
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

}