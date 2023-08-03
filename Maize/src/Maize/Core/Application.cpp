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

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
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
				OnEvent();

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

	void Application::OnEvent()
	{
		SDL_Event event;

		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				m_Running = false;
			}
		}
	}

}