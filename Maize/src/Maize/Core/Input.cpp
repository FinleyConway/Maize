#include "mpch.h"
#include "Maize/Core/Input.h"

#include "Maize/Core/KeyCodes.h"
#include "Maize/Layers/InputLayer.h"

namespace Maize {

	bool Input::GetButtonDown(KeyCode key)
	{
		auto& instance = s_InputHandler;

		bool prevState = instance->m_PrevKeyDown[static_cast<size_t>(key)];
		bool currentState = instance->m_KeyDown[static_cast<size_t>(key)];

		// assign the previous key pressed
		instance->m_PrevKeyDown[static_cast<size_t>(key)] = currentState;

		return currentState && !prevState;
	}

	bool Input::GetButton(KeyCode key)
	{
		return s_InputHandler->m_KeyDown[static_cast<size_t>(key)];
	}

	bool Input::GetButtonUp(KeyCode key)
	{
		auto& instance = s_InputHandler;

		bool prevState = instance->m_PrevKeyUp[static_cast<size_t>(key)];
		bool currentState = instance->m_KeyUp[static_cast<size_t>(key)];

		// assign the previous key pressed
		instance->m_PrevKeyUp[static_cast<size_t>(key)] = currentState;

		return currentState && !prevState;
	}

	bool Input::GetMouseButtonDown(MouseCode button)
	{
		auto& instance = s_InputHandler;

		bool prevState = instance->m_PrevMouseDown[static_cast<size_t>(button)];
		bool currentState = instance->m_MouseDown[static_cast<size_t>(button)];

		// assign the previous button pressed
		instance->m_PrevMouseDown[static_cast<size_t>(button)] = currentState;

		return currentState && !prevState;
	}

	bool Input::GetMouseButton(MouseCode button)
	{
		auto& instance = s_InputHandler;

		return instance->m_MouseDown[static_cast<size_t>(button)];
	}

	bool Input::GetMouseButtonUp(MouseCode button)
	{
		auto& instance = s_InputHandler;

		bool prevState = instance->m_PrevMouseUp[static_cast<size_t>(button)];
		bool currentState = instance->m_MouseUp[static_cast<size_t>(button)];

		// assign the previous button pressed
		instance->m_PrevMouseUp[static_cast<size_t>(button)] = currentState;

		return currentState && !prevState;
	}

	Vector2 Input::GetMousePosition()
	{
		return s_InputHandler->m_MousePosition;
	}

	void Input::SetInputLayer(InputLayer* inputLayer)
	{
		s_InputHandler = inputLayer;
	}

} // Maize