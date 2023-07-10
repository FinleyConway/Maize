#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include <iostream>

#include "Point.h"

namespace Maize {

	class Window
	{
	public:
		Window(const std::string& title, Point windowPosition, Point windowSize, uint32_t flags);

		std::string Title() const;
		void Title(const std::string& title) const;

		Point Position() const;
		void Position(Point position) const;

		Point Size() const;
		void Size(Point size) const;

		void Maximize() const;
		void Minimize() const;

		operator SDL_Window* () const;

	private:
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window{ nullptr, &SDL_DestroyWindow };

		friend class Renderer;
	};

}