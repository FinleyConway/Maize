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
			else if (sdlEvent.type == SDL_WINDOWEVENT)
			{
				if (sdlEvent.window.type == SDL_WINDOWEVENT_RESIZED)
				{
					m_WindowData.width = sdlEvent.window.data1;
					m_WindowData.height = sdlEvent.window.data2;

					WindowResizeEvent event(sdlEvent.window.data1, sdlEvent.window.data2);
					m_WindowData.eventCallback(event);
				}
			}
			else if (sdlEvent.type == SDL_KEYDOWN)
			{
				KeyPressedEvent event((KeyCode)sdlEvent.key.keysym.sym);
				m_WindowData.eventCallback(event);
			}
			else if (sdlEvent.type == SDL_KEYUP)
			{
				KeyReleasedEvent event((KeyCode)sdlEvent.key.keysym.sym);
				m_WindowData.eventCallback(event);
			}
			else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN)
			{
				MouseButtonPressedEvent event((MouseCode)sdlEvent.button.button);
				m_WindowData.eventCallback(event);
			}
			else if (sdlEvent.type == SDL_MOUSEBUTTONUP)
			{
				MouseButtonReleasedEvent event((MouseCode)sdlEvent.button.button);
				m_WindowData.eventCallback(event);
			}
			else if (sdlEvent.type == SDL_MOUSEMOTION)
			{
				MouseMovedEvent event((float)sdlEvent.motion.x, (float)sdlEvent.motion.y);
				m_WindowData.eventCallback(event);
			}
			else if (sdlEvent.type == SDL_MOUSEWHEEL)
			{
				MouseScrolledEvent event(sdlEvent.wheel.preciseX, sdlEvent.wheel.preciseY);
				m_WindowData.eventCallback(event);
			}
		}
	}

	void Window::SetTitle(const std::string& title)
	{
		m_WindowData.title = title;
		SDL_SetWindowTitle(m_Window.get(), title.c_str());
	}

	Window::operator SDL_Window* () const
	{
		return m_Window.get();
	}

}