#include "mpch.h"
#include "Maize/Core/Input.h"

#include "Maize/Events/Event.h"
#include "Maize/Events/EventDispatcher.h"
#include "Maize/Events/KeyEvents.h"
#include "Maize/Events/MouseEvents.h"

namespace Maize {

	Input& Input::Instance()
	{
		static Input instance;
		return instance;
	}

	bool Input::GetButtonDown(KeyCode key)
	{
		auto& instance = Instance();

		bool prevState = instance.m_PrevKeyDown[static_cast<size_t>(key)];
		bool currentState = instance.m_KeyDown[static_cast<size_t>(key)];

		// assign the previous key pressed
		instance.m_PrevKeyDown[static_cast<size_t>(key)] = currentState;

		return currentState && !prevState;
	}

	bool Input::GetButton(KeyCode key)
	{
		auto& instance = Instance();

		return instance.m_KeyDown[static_cast<size_t>(key)];
	}

	bool Input::GetButtonUp(KeyCode key)
	{
		auto& instance = Instance();

		bool prevState = instance.m_PrevKeyUp[static_cast<size_t>(key)];
		bool currentState = instance.m_KeyUp[static_cast<size_t>(key)];

		// assign the previous key pressed
		instance.m_PrevKeyUp[static_cast<size_t>(key)] = currentState;

		return currentState && !prevState;
	}

	bool Input::GetMouseButtonDown(MouseCode button)
	{
		auto& instance = Instance();

		bool prevState = instance.m_PrevMouseDown[static_cast<size_t>(button)];
		bool currentState = instance.m_MouseDown[static_cast<size_t>(button)];

		// assign the previous button pressed
		instance.m_PrevMouseDown[static_cast<size_t>(button)] = currentState;

		return currentState && !prevState;
	}

	bool Input::GetMouseButton(MouseCode button)
	{
		auto& instance = Instance();

		return instance.m_MouseDown[static_cast<size_t>(button)];
	}

	bool Input::GetMouseButtonUp(MouseCode button)
	{
		auto& instance = Instance();

		bool prevState = instance.m_PrevMouseUp[static_cast<size_t>(button)];
		bool currentState = instance.m_MouseUp[static_cast<size_t>(button)];

		// assign the previous button pressed
		instance.m_PrevMouseUp[static_cast<size_t>(button)] = currentState;

		return currentState && !prevState;
	}

	Vector2 Input::GetMousePosition()
	{
		return Instance().m_MousePosition;
	}

	void Input::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&Input::OnKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&Input::OnKeyReleased, this, std::placeholders::_1));

		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&Input::OnMouseMoved, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&Input::OnMousePressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&Input::OnMouseReleased, this, std::placeholders::_1));
	}

	bool Input::OnKeyPressed(const KeyPressedEvent& e)
	{
		auto& instance = Instance();
		auto codeIndex = static_cast<size_t>(e.GetKeyCode());

		instance.m_KeyDown[codeIndex] = true;
		instance.m_KeyUp[codeIndex] = false;

		return false;
	}

	bool Input::OnKeyReleased(const KeyReleasedEvent& e)
	{
		auto& instance = Instance();
		auto codeIndex = static_cast<size_t>(e.GetKeyCode());

		instance.m_KeyDown[codeIndex] = false;
		instance.m_KeyUp[codeIndex] = true;

		return false;
	}

	bool Input::OnMouseMoved(const MouseMovedEvent& e)
	{
		auto& instance = Instance();

		instance.m_MousePosition = Vector2(e.GetX(), e.GetY());

		return false;
	}

	bool Input::OnMousePressed(const MouseButtonPressedEvent& e)
	{
		auto& instance = Instance();
		auto codeIndex = static_cast<size_t>(e.GetMouseButton());

		instance.m_MouseDown[codeIndex] = true;
		instance.m_MouseUp[codeIndex] = false;

		return false;
	}

	bool Input::OnMouseReleased(const MouseButtonReleasedEvent& e)
	{
		auto& instance = Instance();
		auto codeIndex = static_cast<size_t>(e.GetMouseButton());

		instance.m_MouseDown[codeIndex] = false;
		instance.m_MouseUp[codeIndex] = true;

		return false;
	}

} // Maize