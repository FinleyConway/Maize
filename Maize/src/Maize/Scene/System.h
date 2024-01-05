#pragma once

#include <entt/entt.hpp>

namespace Maize {

	class System
	{
	public:
		virtual ~System() = default;

		virtual void Start(entt::registry& registry) = 0;
		virtual void PhysicsUpdate(entt::registry& registry, float deltaTime) = 0;
		virtual void PhysicsCallback(entt::registry& registry, float deltaTime) = 0;
		virtual void Update(entt::registry& registry, float deltaTime) = 0;
		virtual void LateUpdate(entt::registry& registry, float deltaTime) = 0;
		virtual void OnEnd(entt::registry& registry) = 0;
	};

} // Maize
