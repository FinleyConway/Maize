#pragma once

#include <string>

#include "Maize/Events/Event.h"

namespace Maize {

	class Layer
	{
	public:
		explicit Layer(const std::string& name = "Layer") : m_DebugName(name) { }
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& Name() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}