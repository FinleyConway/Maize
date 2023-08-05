#pragma once

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_sdlrenderer2.h>

#include "Maize/Core/Layer.h"

namespace Maize {

	class ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer() : Layer("ImGuiLayer") { }
		~ImGuiLayer() = default;

		void OnAttach() override;
		void OnDetach() override;

		void Begin();
		void End();

	private:
	};

}