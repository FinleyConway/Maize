#pragma once

#include <entt/entt.hpp>

namespace Maize {

	class System
	{
	public:
		virtual ~System() = default;

		virtual void Initialize(entt::registry& registry) { };
		virtual void PhysicsUpdate(entt::registry& registry, float deltaTime) { };
		virtual void PhysicsCallback(entt::registry& registry, float deltaTime) { };
		virtual void Update(entt::registry& registry, float deltaTime) { };
		virtual void LateUpdate(entt::registry& registry, float deltaTime) { };
		virtual void Shutdown(entt::registry& registry) { };
	};

} // Maize
