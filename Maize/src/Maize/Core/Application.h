#pragma once

#include "Maize/Events/EventDispatcher.h"
#include "Maize/Events/WindowEvents.h"
#include "Maize/ImGui/ImGuiLayer.h"
#include "Maize/Core/LayerStack.h"
#include "Maize/Core/Window.h"
#include "Maize/Math/Point.h"
#include "Maize/Core/Layer.h"

int main();

namespace Maize {

	struct ApplicationSpecification
	{
		std::string name = "Maize";
	};

	class Application
	{
	public:
		explicit Application(const ApplicationSpecification& specification);

		static Application& Get() { return *s_Instance; }

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		Window& GetWindow() { return m_Window; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		void Close();

	private:
		static Application* s_Instance;
		friend int ::main();

		Window m_Window;

		LayerStack m_LayerStack;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		void Run();

		bool OnWindowClose(WindowCloseEvent& e);
	};

	// init engine 
	Application* CreateApplication();

}