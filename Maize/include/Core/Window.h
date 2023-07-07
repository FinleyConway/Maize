#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include <iostream>

#include "Math/Vec2Int.h"

namespace Maize {

	class Window
	{
	public:
		Window(const std::string& title, Vec2Int windowPosition, Vec2Int windowSize, uint32_t flags);

		std::string Title() const;
		void Title(const std::string& title);

		Vec2Int Position() const;
		void Position(Vec2Int position);

		Vec2Int Size() const;
		void Size(Vec2Int size);

		void Maximize() const;
		void Minimize() const;

		operator SDL_Window* () const;

	private:
		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window{ nullptr, &SDL_DestroyWindow };

		friend class Renderer;
	};

}