#include "mpch.h"
#include "Maize/Layers/InputLayer.h"

#include "Maize/Events/EventDispatcher.h"
#include "Maize/Events/KeyEvents.h"
#include "Maize/Events/MouseEvents.h"

namespace Maize {

	InputLayer::InputLayer()
	{
		m_Input.SetInputLayer(this);
	}

	void InputLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);

		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&InputLayer::OnKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&InputLayer::OnKeyReleased, this, std::placeholders::_1));

		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&InputLayer::OnMouseMoved, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&InputLayer::OnMousePressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&InputLayer::OnMouseReleased, this, std::placeholders::_1));
	}

	bool InputLayer::OnKeyPressed(const KeyPressedEvent& e)
	{
		auto codeIndex = static_cast<size_t>(e.GetKeyCode());

		m_KeyDown[codeIndex] = true;
		m_KeyUp[codeIndex] = false;

		return false;
	}

	bool InputLayer::OnKeyReleased(const KeyReleasedEvent& e)
	{
		auto codeIndex = static_cast<size_t>(e.GetKeyCode());

		m_KeyDown[codeIndex] = false;
		m_KeyUp[codeIndex] = true;

		return false;
	}

	bool InputLayer::OnMouseMoved(const MouseMovedEvent& e)
	{
		m_MousePosition = Vector2(static_cast<float>(e.GetX()), static_cast<float>(e.GetY()));

		return false;
	}

	bool InputLayer::OnMousePressed(const MouseButtonPressedEvent& e)
	{
		auto codeIndex = static_cast<size_t>(e.GetMouseButton());

		m_MouseDown[codeIndex] = true;
		m_MouseUp[codeIndex] = false;

		return false;
	}

	bool InputLayer::OnMouseReleased(const MouseButtonReleasedEvent& e)
	{
		auto codeIndex = static_cast<size_t>(e.GetMouseButton());

		m_MouseDown[codeIndex] = false;
		m_MouseUp[codeIndex] = true;

		return false;
	}

} // Maize