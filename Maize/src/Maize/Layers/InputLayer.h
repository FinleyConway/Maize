#pragma once

#include "Maize/Core/Layer.h"
#include "Maize/Utils/Vector2.h"

#include "Maize/Core/Input.h"

namespace Maize {

	class KeyPressedEvent;
	class KeyReleasedEvent;
	class MouseMovedEvent;
	class MouseButtonPressedEvent;
	class MouseButtonReleasedEvent;
	class Event;

	class InputLayer final : public Layer
	{
	 public:
		explicit InputLayer(Input& input);

		void OnEvent(Event& e) override;

	 private:
		friend class Input;

		bool OnKeyPressed(const KeyPressedEvent& e);
		bool OnKeyReleased(const KeyReleasedEvent& e);
		bool OnMouseMoved(const MouseMovedEvent& e);
		bool OnMousePressed(const MouseButtonPressedEvent& e);
		bool OnMouseReleased(const MouseButtonReleasedEvent& e);

	 private:
		std::array<bool, sf::Keyboard::KeyCount> m_KeyDown{};
		std::array<bool, sf::Keyboard::KeyCount> m_KeyUp{};
		std::array<bool, sf::Keyboard::KeyCount> m_PrevKeyDown{};
		std::array<bool, sf::Keyboard::KeyCount> m_PrevKeyUp{};

		std::array<bool, sf::Mouse::ButtonCount> m_MouseDown{};
		std::array<bool, sf::Mouse::ButtonCount> m_MouseUp{};
		std::array<bool, sf::Mouse::ButtonCount> m_PrevMouseDown{};
		std::array<bool, sf::Mouse::ButtonCount> m_PrevMouseUp{};

		Vector2 m_MousePosition;
	};

} // Maize
