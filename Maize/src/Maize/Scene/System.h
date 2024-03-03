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

		void SetOrderPriority(uint32_t orderPriority) { m_OrderPriority = orderPriority; }

		bool operator<(const System& system) const
		{
			return m_OrderPriority < system.m_OrderPriority;
		}

	 private:
		uint32_t m_OrderPriority = 0;
	};

} // Maize
