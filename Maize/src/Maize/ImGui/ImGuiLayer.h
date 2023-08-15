#pragma once

#include <imgui.h>
#include <imgui-SFML.h>

#include "Maize/Core/Layer.h"

namespace Maize {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") { }
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnEvent(Event& e) override;
		void OnDetach() override;

		void Begin(sf::Time time) const;
		void End() const;
	};

}