#pragma once

#include <functional>

#include "Maize/Events/EventDispatcher.h"
#include "Maize/Events/MouseEvent.h"
#include "Maize/Events/KeyEvent.h"
#include "Maize/Core/Layer.h"
#include "Maize/Core/Input.h"

namespace Maize {

	class InputPollingLayer : public Layer
	{
	public:
		void OnEvent(Event& e) override;
		void End() const;

		void OnKeyPressed(KeyPressedEvent& e) const;
		void OnKeyReleased(KeyReleasedEvent& e) const;

		void OnMousePressed(MouseButtonPressedEvent& e) const;
		void OnMouseReleased(MouseButtonReleasedEvent& e) const;
		void OnMouseScrolled(MouseScrolledEvent& e) const;
		void OnMouseMoved(MouseMovedEvent& e) const;
	};

}