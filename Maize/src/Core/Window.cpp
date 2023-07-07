#include "Core/Window.h"

namespace Maize {

	Window::Window(const std::string& title, Vec2Int windowPosition, Vec2Int windowSize, uint32_t flags)
	{
		m_Window.reset(SDL_CreateWindow(title.c_str(), windowPosition.x, windowPosition.y, windowSize.x, windowSize.y, flags));
		if (m_Window == nullptr)
		{
			std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
		}
	}

	std::string Window::Title() const
	{
		return SDL_GetWindowTitle(m_Window.get());
	}

	void Window::Title(const std::string& title)
	{
		SDL_SetWindowTitle(m_Window.get(), title.c_str());
	}

	Vec2Int Window::Position() const
	{
		int x, y;
		SDL_GetWindowPosition(m_Window.get(), &x, &y);
		return { x, y };
	}

	void Window::Position(Vec2Int position)
	{
		SDL_SetWindowPosition(m_Window.get(), position.x, position.y);
	}

	Vec2Int Window::Size() const
	{
		int x, y;
		SDL_GetWindowSize(m_Window.get(), &x, &y);
		return { x, y };
	}

	void Window::Size(Vec2Int size)
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