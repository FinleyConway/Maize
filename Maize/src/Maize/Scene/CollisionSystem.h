#pragma once

#include <box2d/box2d.h>
#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Scene/Components.h"

namespace Maize {

	class CollisionSystem
	{
	public:
		void OnStart(ECS::EntityWorld& reg);
		void OnUpdate(ECS::EntityWorld& reg, float deltaTime);
		void OnDestroy();

		b2World* m_PhysicsWorld;

	private:
		void UpdateBox2d(ECS::EntityWorld& reg);
		float NormalizeAngle(float angle) {
			while (angle > 180.0f) {
				angle -= 360.0f;
			}

			while (angle < -180.0f) {
				angle += 360.0f;
			}

			return angle;
		}

	};

} // Maize
