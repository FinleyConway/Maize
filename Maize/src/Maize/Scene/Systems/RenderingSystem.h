#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/System.h"

namespace Maize {

	class RenderingSystem : public System
	{
	public:
		void Initialize(entt::registry& registry) override;
		void Update(entt::registry& registry, float update) override;
		void Shutdown(entt::registry& registry) override;
	};

} // Maize
