#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/System.h"

namespace Maize {

	struct Transform;

	class HierarchySystem : public System
	{
	 public:
		void Update(entt::registry& registry, float deltaTime) override;
		void UpdateEntityPositions(entt::registry& registry, entt::entity entity, const Transform& parentTransform);
	};

} // Maize
