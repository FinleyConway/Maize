#include "Maize/Events/InputPollingLayer.h"

namespace Maize {

	void InputPollingLayer::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(std::bind(&InputPollingLayer::OnKeyPressed, this, std::placeholders::_1));
		dispatcher.Dispatch<KeyReleasedEvent>(std::bind(&InputPollingLayer::OnKeyReleased, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&InputPollingLayer::OnMousePressed, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&InputPollingLayer::OnMouseReleased, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseScrolledEvent>(std::bind(&InputPollingLayer::OnMouseScrolled, this, std::placeholders::_1));
		dispatcher.Dispatch<MouseMovedEvent>(std::bind(&InputPollingLayer::OnMouseMoved, this, std::placeholders::_1));
	}

	void InputPollingLayer::End() const
	{
		Input::m_PreviousMouseState = Input::m_CurrentMouseState;
		Input::m_PreviousKeyboardState = Input::m_CurrentKeyboardState;
	}

	void InputPollingLayer::OnKeyPressed(KeyPressedEvent& e) const
	{
		Input::m_CurrentKeyboardState[e.GetKeyCode()] = true;
	}

	void InputPollingLayer::OnKeyReleased(KeyReleasedEvent& e) const
	{
		Input::m_CurrentKeyboardState[e.GetKeyCode()] = false;
	}

	void InputPollingLayer::OnMousePressed(MouseButtonPressedEvent& e) const
	{
		Input::m_CurrentMouseState[e.GetMouseButton()] = true;
	}

	void InputPollingLayer::OnMouseReleased(MouseButtonReleasedEvent& e) const
	{
		Input::m_CurrentMouseState[e.GetMouseButton()] = false;
	}

	void InputPollingLayer::OnMouseScrolled(MouseScrolledEvent& e) const
	{
		Input::m_MouseScrollDelta = PointF(e.GetXOffset(), e.GetYOffset());
	}

	void InputPollingLayer::OnMouseMoved(MouseMovedEvent& e) const
	{
		Input::m_MousePosition = PointF(e.GetX(), e.GetY());
	}

}