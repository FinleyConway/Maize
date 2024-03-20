#pragma once

namespace Maize {

	class Renderer;
	class Event;

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		explicit Window(const std::string& title, sf::Vector2u windowSize = { 1280, 720 });

		void InitializeRenderer(Renderer& renderer);

		void SetTitle(const std::string& title);
		const std::string& GetTitle() const;
		sf::Vector2u GetSize() const;
		void SetVSync(bool enable);
		bool IsVSyncEnabled() const;

		void ToggleFullscreen();

		void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }
		void PollEvents();

	private:
		void Create(sf::Vector2u windowSize);

	private:
		sf::RenderWindow m_Window;
		std::string m_WindowTitle;
		bool m_IsFullscreen = false;
		bool m_IsVSyncEnabled = false;
		EventCallbackFn m_EventCallback;
	};

} // Maize