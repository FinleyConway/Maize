#pragma once

#include <EntityComponentSystem/EntityWorld.h>

namespace Maize {

	class CollisionSystem
	{
	public:
		static void OnStart(ECS::EntityWorld& reg);
		static void OnUpdate(ECS::EntityWorld& reg, float deltaTime);
		static void OnDestroy();

	private:
		static void UpdateBox2d(ECS::EntityWorld& reg);
		static void UpdateECS(ECS::EntityWorld& reg);

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
