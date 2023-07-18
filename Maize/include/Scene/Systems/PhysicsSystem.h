#pragma once

#include <box2d.h>
#include <memory>

#include "System.h"
#include "Core/PointF.h"
#include "Scene/Components.h"
#include "Scene/CollisionContactListener.h"

namespace Maize {

	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem(ECS::EntityWorld& registry, PointF gravity);

		void OnStart(ECS::EntityWorld& registry) override;
		void OnUpdate(ECS::EntityWorld& registry, float dt) override;
		void OnEndFrame(ECS::EntityWorld& registry) override;

	private:
		std::unique_ptr<b2World> m_World;
		CollisionContactListener m_ContactListener;
	};

}