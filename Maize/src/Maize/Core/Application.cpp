#include "Maize/Core/Application.h"

namespace Maize {

	Application* Application::s_Instance = nullptr;

	Application::Application() : 
		m_Renderer(m_Window)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return;
		if (~IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) return;
		if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) return;
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return;

		s_Instance = this;

		m_Window.SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
		while (m_Running)
		{
			// temp - will make a time class
			float delta = 0.01666666666f;

			if (!m_Minimized)
			{
				m_Window.PollEvent();

				for (Layer* layer : m_LayerStack)
					layer->OnUpdate(delta);

				m_ImGuiLayer->Begin();

				m_Renderer.Clear();

				for (Layer* layer : m_LayerStack)
					layer->OnRender();

				m_ImGuiLayer->End();

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