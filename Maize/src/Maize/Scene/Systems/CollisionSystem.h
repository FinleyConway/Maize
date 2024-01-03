#pragma once

#include <entt/entt.hpp>

namespace Maize {

	class CollisionSystem
	{
	public:
		static void OnStart(entt::registry& reg);
		static void OnUpdate(entt::registry& reg, float deltaTime);
		static void OnDestroy();

	private:
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
