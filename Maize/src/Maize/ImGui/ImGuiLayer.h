#pragma once

#include <imgui.h>
#include <imgui-SFML.h>
#include <iostream>

#include "Maize/Core/Window.h"
#include "Maize/Core/Layer.h"

namespace Maize {

    class ImGuiLayer : public Layer
    {
    public:
        ~ImGuiLayer() = default;

        void OnAttach() override;
        void OnEvent(Event& e) override;
        void OnDetach() override;

        void Begin(Window& window, float deltaTime) const;
        void End(Window& window) const;
    };

} // Maize