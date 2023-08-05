#include "Maize/Core/Window.h"

namespace Maize {

	Window::Window(const std::string& title, Point windowSize)
	{
		m_Window.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowSize.x, windowSize.y, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
		if (m_Window == nullptr)
		{
			std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		}

		m_WindowData.title = title;
		m_WindowData.width = windowSize.x;
		m_WindowData.height = windowSize.y;

		SetVSync(true);
	}

	void Window::SetVSync(bool enable)
	{
		if (enable)
			SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
		else
			SDL_SetHint(SDL_HINT_RENDER_VSYNC, "0");
		m_WindowData.vSync = enable;
	}

	void Window::PollEvent()
	{
		SDL_Event sdlEvent;

		while (SDL_PollEvent(&sdlEvent))
		{
			ImGui_ImplSDL2_ProcessEvent(&sdlEvent); // kinda sucks that it has to be here but it is what it is

			if (sdlEvent.type == SDL_QUIT)
			{
				WindowCloseEvent event;
				m_WindowData.eventCallback(event);
			}
		}
	}

	void Window::Title(const std::string& title) const
	{
		SDL_SetWindowTitle(m_Window.get(), title.c_str());
	}

	Window::operator SDL_Window* () const
	{
		return m_Window.get();
	}

}