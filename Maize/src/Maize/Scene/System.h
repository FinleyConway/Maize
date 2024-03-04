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
		uint32_t GetOrderPriority() const { return m_OrderPriority; }

		void SetName(const std::string& name) { m_Name = name; }
		const std::string& GetName() const { return m_Name; }

	 private:
		uint32_t m_OrderPriority;
		std::string m_Name; // debug purposes
	};

} // Maize
