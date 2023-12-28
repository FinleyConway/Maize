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

	private:
		void UpdateBox2d(ECS::EntityWorld& reg);

		b2World* m_PhysicsWorld;
	};

} // Maize
