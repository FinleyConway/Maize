#pragma once

#include <entt/entt.hpp>
#include <box2d/box2d.h>

namespace Maize {

	struct BodyUserData;

	class PhysicsContactListener : public b2ContactListener
	{
	public:
		entt::registry* registry = nullptr;

		void BeginContact(b2Contact* contact) override;
		void EndContact(b2Contact* contact) override;

	private:
		void HandleContact(b2Contact* contact, bool begin);
		void AddCollisionComponents(bool begin, bool sensor, entt::entity entityA, entt::entity entityB) const;
	};

} // Maize