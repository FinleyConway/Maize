#include "Core/Application.h"

namespace Maize {

	Application::Application() : 
		m_Renderer(m_Window)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) return;
		if (~IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) return;
		if (Mix_Init(MIX_INIT_MP3) != MIX_INIT_MP3) return;
		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return;

		Run();
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

				OnUpdate(delta);

				OnRender();
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

	void Application::OnUpdate(float dt)
	{
	}

	void Application::OnRender()
	{
		m_Renderer.Clear();

		// render

		m_Renderer.Present();
	}

}