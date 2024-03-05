#pragma once

#include "Maize/Core/KeyCodes.h"
#include "Maize/Math/Vector2.h"

namespace Maize {

	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseMovedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class Event;

	class Input
	{
	 public:
		static bool GetButtonDown(KeyCode key);
		static bool GetButton(KeyCode key);
		static bool GetButtonUp(KeyCode key);

		static Vector2 GetMousePosition();

		static bool GetMouseButtonDown(MouseCode button);
		static bool GetMouseButton(MouseCode button);
		static bool GetMouseButtonUp(MouseCode button);

		void OnEvent(Event& e);

	 private:
		static Input& Instance();

		bool OnKeyPressed(const KeyPressedEvent& e);
		bool OnKeyReleased(const KeyReleasedEvent& e);
		bool OnMouseMoved(const MouseMovedEvent& e);
		bool OnMousePressed(const MouseButtonPressedEvent& e);
		bool OnMouseReleased(const MouseButtonReleasedEvent& e);

	 private:
		std::array<bool, sf::Keyboard::KeyCount> m_KeyDown;
		std::array<bool, sf::Keyboard::KeyCount> m_KeyUp;
		std::array<bool, sf::Keyboard::KeyCount> m_PrevKeyDown;
		std::array<bool, sf::Keyboard::KeyCount> m_PrevKeyUp;

		std::array<bool, sf::Mouse::ButtonCount> m_MouseDown;
		std::array<bool, sf::Mouse::ButtonCount> m_MouseUp;
		std::array<bool, sf::Mouse::ButtonCount> m_PrevMouseDown;
		std::array<bool, sf::Mouse::ButtonCount> m_PrevMouseUp;

		Vector2 m_MousePosition;
	};

} // Maize
