#pragma once

#include <string>

#include "Maize/Events/Event.h"

namespace Maize {

    class Layer
    {
    public:
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnEvent(Event& event) {}
    };

} // Maize