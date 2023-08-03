#pragma once

#include <box2d.h>
#include <memory>

#include "Maize/Scene/CollisionContactListener.h"
#include "Maize/EntitySystems/System.h"
#include "Maize/Scene/Components.h"
#include "Maize/Math/PointF.h"

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