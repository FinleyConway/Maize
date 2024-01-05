#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/Components.h"
#include "Maize/Renderer/Renderer.h"

namespace Maize {

	class CameraSystem
	{
	public:
		static void OnUpdate(entt::registry& reg, float deltaTime);
	};

} // Maize
