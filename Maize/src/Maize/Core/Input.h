#pragma once

namespace Maize {

	enum class KeyCode;
	enum class MouseCode;
	class InputLayer;
	class Vector2;

	class Input final
	{
	 public:
		bool GetButtonDown(KeyCode key);
		bool GetButton(KeyCode key);
		bool GetButtonUp(KeyCode key);

		Vector2 GetMousePosition();

		bool GetMouseButtonDown(MouseCode button);
		bool GetMouseButton(MouseCode button);
		bool GetMouseButtonUp(MouseCode button);

	 private:
		friend class InputLayer;

		void SetInputLayer(InputLayer* inputLayer);

		InputLayer* m_InputHandler = nullptr;
	};

} // Maize
