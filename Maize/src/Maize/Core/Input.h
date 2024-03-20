#pragma once

namespace Maize {

	enum class KeyCode;
	enum class MouseCode;
	class InputLayer;
	class Vector2;

	class Input final
	{
	 public:
		static bool GetButtonDown(KeyCode key);
		static bool GetButton(KeyCode key);
		static bool GetButtonUp(KeyCode key);

		static Vector2 GetMousePosition();

		static bool GetMouseButtonDown(MouseCode button);
		static bool GetMouseButton(MouseCode button);
		static bool GetMouseButtonUp(MouseCode button);

	 private:
		friend class InputLayer;

		void SetInputLayer(InputLayer* inputLayer);

		inline static InputLayer* s_InputHandler = nullptr;
	};

} // Maize
