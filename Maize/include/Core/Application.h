#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Point.h"
#include "Window.h"
#include "Renderer.h"

namespace Maize {

	class Application
	{
	public:
		Application();

		void Close();

	private:
		Window m_Window = { "Test", Point(50, 50), Point(640, 480), 0 };
		Renderer m_Renderer;

		bool m_Running = true;
		bool m_Minimized = false;

		void Run();
		void OnEvent();
		void OnUpdate(float dt);
		void OnRender();
	};

}