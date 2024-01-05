#pragma once

#include <entt/entt.hpp>

#include "Maize/Physics/PhysicsContactListener.h"

namespace Maize {

	struct BodyUserData
	{
		entt::entity attachedEntity = entt::null;
	};

	class CollisionSystem
	{
	public:
		static void OnStart(entt::registry& reg);
		static void OnUpdate(entt::registry& reg, float deltaTime);
		static void OnDestroy();

	private:
		inline static PhysicsContactListener s_ContactListener;

		static void UpdateBox2d(entt::registry& reg);
		static void UpdateECS(entt::registry& reg);

		static float NormalizeAngle(float angle)
		{
			while (angle > 180.0f)
			{
				angle -= 360.0f;
			}

			while (angle < -180.0f)
			{
				angle += 360.0f;
			}

			return angle;
		}

	};

} // Maize
