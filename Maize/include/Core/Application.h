#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Window.h"
#include "Math/Point.h"
#include "Renderer/Renderer.h"

int main(int argc, char** argv);

namespace Maize {

	class Application
	{
	public:
		Application();
		virtual ~Application() = default;

		static Application& Get() { return *s_Instance; }

		Window& GetWindow() { return m_Window; }

		void Close();

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);

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