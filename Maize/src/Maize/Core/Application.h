#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "Maize/Renderer/Renderer.h"
#include "Maize/ImGui/ImGuiLayer.h"
#include "Maize/Core/LayerStack.h"
#include "Maize/Core/Window.h"
#include "Maize/Math/Point.h"
#include "Maize/Core/Layer.h"

int main(int argc, char* argv[]);

namespace Maize {

	class Application
	{
	public:
		Application();
		virtual ~Application();

		static Application& Get() { return *s_Instance; }

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return m_Window; }
		Renderer& GetRenderer() { return m_Renderer; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		void Close();

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char* argv[]);

		Window m_Window = { "Test", Point(50, 50), Point(640*2, 480*2), 0 };
		Renderer m_Renderer;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		void Run();
		void OnEvent();
	};

	// init engine 
	Application* CreateApplication();

}