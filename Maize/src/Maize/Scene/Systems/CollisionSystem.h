#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/System.h"

#include "Maize/Physics/PhysicsEngine.h"
#include "Maize/Physics/PhysicsContactListener.h"

namespace Maize {

	struct BodyUserData
	{
		entt::entity attachedEntity = entt::null;
	};

	class CollisionSystem : public System
	{
	 public:
		void Initialize(entt::registry& registry) override;
		void Update(entt::registry& registry, float deltaTime) override;
		void Shutdown(entt::registry& registry) override;

	 private:
		void UpdateBox2d(entt::registry& reg);
		void UpdateECS(entt::registry& reg);

	 private:
		PhysicsEngine m_PhysicsEngine;
		PhysicsContactListener m_ContactListener;
	};

} // Maize
