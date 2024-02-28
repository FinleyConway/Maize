#pragma once

#include <entt/entt.hpp>

namespace Maize {

	class RenderingSystem
	{
	public:
		void OnStart(entt::registry& registry);
		void OnUpdate(entt::registry& registry, float update);
		void OnRender(entt::registry& registry);
	};

} // Maize
