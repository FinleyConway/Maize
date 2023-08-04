#pragma once

#include <SDL.h>

namespace Maize {

	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
									      EventType GetEventType() const override { return GetStaticType(); }

	enum class EventType
	{
		None,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	class Event
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0;
	};

}