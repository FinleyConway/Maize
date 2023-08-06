#pragma once

#include <unordered_map>

#include "Maize/Core/KeyCode.h"
#include "Maize/Core/MouseCode.h"
#include "Maize/Math/PointF.h"

namespace Maize {

	class Input
	{
	public:
		static bool GetAnyKey();
		static bool GetAnyKeyDown();

		static bool GetButton(KeyCode key);
		static bool GetButtonDown(KeyCode key);
		static bool GetButtonUp(KeyCode key);

		static bool GetMouseButton(MouseCode button);
		static bool GetMouseButtonDown(MouseCode button);
		static bool GetMouseButtonUp(MouseCode button);
		static PointF GetMousePosition();
		static PointF GetMouseScrollDelta();

	private:
		friend class InputPollingLayer;

		// input should not be created
		Input() = default;
		Input(const Input&) = delete;
		Input& operator=(const Input&) = delete;

		static std::unordered_map<KeyCode, bool> m_CurrentKeyboardState;
		static std::unordered_map<KeyCode, bool> m_PreviousKeyboardState;
		static std::unordered_map<MouseCode, bool> m_CurrentMouseState;
		static std::unordered_map<MouseCode, bool> m_PreviousMouseState;
		static PointF m_MousePosition;
		static PointF m_MouseScrollDelta;
	};

}