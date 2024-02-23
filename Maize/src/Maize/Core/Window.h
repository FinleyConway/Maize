#pragma once

namespace Maize {

	class Event;

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		explicit Window(const std::string& title, sf::Vector2u windowSize = { 1280, 720 });

		void SetTitle(const std::string& title);
		const std::string& GetTitle() const { return m_WindowTitle; }

		sf::Vector2u GetSize() const { return m_Window.getSize(); }
		float GetAspectRatio() const { return static_cast<float>(m_Window.getSize().x) / static_cast<float>(m_Window.getSize().y); }

		void SetVSync(bool enable);
		bool IsVSyncEnabled() const { return m_IsVSyncEnabled; }

		void SetEventCallback(const EventCallbackFn& callback) { m_EventCallback = callback; }

		void PollEvents();

		void ToggleFullscreen();

		void BeginDrawing(sf::Color clearColour = sf::Color::Black);
		void EndDrawing();

		sf::RenderWindow& GetRenderWindow() { return m_Window; }

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