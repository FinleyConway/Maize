#pragma once

#include <box2d.h>
#include <EntityWorld.h>
#include <vector>

#include "Maize/Scene/Components.h"

namespace Maize {

	class CollisionContactListener : public b2ContactListener
	{
	public:
		explicit CollisionContactListener(ECS::EntityWorld& registry) : m_Registry(registry) { }

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

	private:
		ECS::EntityWorld& m_Registry;
	};

}