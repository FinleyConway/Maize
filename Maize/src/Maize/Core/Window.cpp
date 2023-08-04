#include "Maize/Core/Window.h"

namespace Maize {

	Window::Window(const std::string& title, Point windowPosition, Point windowSize, uint32_t flags)
	{
		m_Window.reset(SDL_CreateWindow(title.c_str(), windowPosition.x, windowPosition.y, windowSize.x, windowSize.y, flags));
		if (m_Window == nullptr)
		{
			std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		}
	}

	void Window::PollEvent()
	{
		SDL_Event sdlEvent;

		while (SDL_PollEvent(&sdlEvent))
		{
			if (sdlEvent.type == SDL_QUIT)
			{
				WindowCloseEvent event;
				m_WindowData.eventCallback(event);
			}
		}
	}

	std::string Window::Title() const
	{
		return SDL_GetWindowTitle(m_Window.get());
	}

	void Window::Title(const std::string& title) const
	{
		SDL_SetWindowTitle(m_Window.get(), title.c_str());
	}

	Point Window::Position() const
	{
		int x, y;
		SDL_GetWindowPosition(m_Window.get(), &x, &y);
		return { x, y };
	}

	void Window::Position(Point position) const
	{
		SDL_SetWindowPosition(m_Window.get(), position.x, position.y);
	}

	Point Window::Size() const
	{
		int x, y;
		SDL_GetWindowSize(m_Window.get(), &x, &y);
		return { x, y };
	}

	void Window::Size(Point size) const
	{
		SDL_SetWindowSize(m_Window.get(), size.x, size.y);
	}

	void Window::Maximize() const
	{
		SDL_MaximizeWindow(m_Window.get());
	}

	void Window::Minimize() const
	{
		SDL_MinimizeWindow(m_Window.get());
	}

	Window::operator SDL_Window* () const
	{
		return m_Window.get();
	}

}