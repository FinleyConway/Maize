#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include <iostream>
#include <functional>
#include <imgui_impl_sdl2.h>

#include "Maize/Events/WindowEvents.h"
#include "Maize/Events/MouseEvent.h"
#include "Maize/Events/KeyEvent.h"
#include "Maize/Events/Event.h"

#include "Maize/Math/Point.h"

namespace Maize {

	class Window
	{
	public:
		using EventCallBackFn = std::function<void(Event&)>;

		Window(const std::string& title, Point windowSize = Point(640, 480));

		std::string GetTitle() const { return m_WindowData.title; }
		uint32_t GetWidth() const { return m_WindowData.width; }
		uint32_t GetHeight() const { return m_WindowData.height; }
		bool IsVSync() const { return m_WindowData.vSync; }

		void SetEventCallback(const EventCallBackFn& callback) { m_WindowData.eventCallback = callback; }
		void SetTitle(const std::string& title);
		void SetVSync(bool enable);

		void PollEvent();

		operator SDL_Window* () const;

	private:
		friend class Renderer;

		std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)> m_Window{ nullptr, &SDL_DestroyWindow };

		struct WindowData
		{
			std::string title = "";
			uint32_t width = 0;
			uint32_t height = 0;
			bool vSync = true;
			EventCallBackFn eventCallback;
		};

		WindowData m_WindowData;
	};

}