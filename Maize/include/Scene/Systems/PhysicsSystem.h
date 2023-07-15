#pragma once

#include <box2d.h>

#include "System.h"
#include "Core/PointF.h"
#include "Scene/Components.h"

namespace Maize {

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem(PointF gravity);

		void OnStart(ECS::EntityWorld& registry) override;
		void OnUpdate(ECS::EntityWorld& registry, float dt) override;

	private:
		b2World* m_World;
	};

}