#include "Maize/Core/Input.h"

namespace Maize {

	std::unordered_map<KeyCode, bool> Input::m_CurrentKeyboardState;
	std::unordered_map<KeyCode, bool> Input::m_PreviousKeyboardState;
	std::unordered_map<MouseCode, bool> Input::m_CurrentMouseState;
	std::unordered_map<MouseCode, bool> Input::m_PreviousMouseState;
	PointF Input::m_MousePosition;
	PointF Input::m_MouseScrollDelta;

	bool Input::GetAnyKey()
	{
		for (const auto& [_, keyState] : m_CurrentKeyboardState) 
		{
			if (keyState)
				return true;
		}

		return false;
	}

	bool Input::GetAnyKeyDown()
	{
		for (const auto& [key, keyState] : m_CurrentKeyboardState) 
		{
			if (keyState && !m_PreviousKeyboardState[key])
				return true;
		}

		return false;
	}

	bool Input::GetButton(KeyCode key)
	{
		return m_CurrentKeyboardState[key];
	}

	bool Input::GetButtonDown(KeyCode key)
	{
		return m_CurrentKeyboardState[key] && !m_PreviousKeyboardState[key];
	}

	bool Input::GetButtonUp(KeyCode key)
	{
		return !m_CurrentKeyboardState[key] && m_PreviousKeyboardState[key];;
	}

	bool Input::GetMouseButton(MouseCode button)
	{
		return m_CurrentMouseState[button];
	}

	bool Input::GetMouseButtonDown(MouseCode button)
	{
		return m_CurrentMouseState[button] && !m_PreviousMouseState[button];
	}

	bool Input::GetMouseButtonUp(MouseCode button)
	{
		return !m_CurrentMouseState[button] && m_PreviousMouseState[button];
	}

	PointF Input::GetMousePosition()
	{
		return m_MousePosition;
	}

	PointF Input::GetMouseScrollDelta()
	{
		return m_MouseScrollDelta;
	}

}