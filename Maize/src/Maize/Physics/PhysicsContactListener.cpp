#include "mpch.h"
#include "Maize/Physics/PhysicsContactListener.h"

#include "Maize/Scene/Components.h"
#include "Maize/Scene/Systems/CollisionSystem.h"

namespace Maize {

	void PhysicsContactListener::BeginContact(b2Contact*contact)
	{
		HandleContact(contact, true);
	}

	void PhysicsContactListener::EndContact(b2Contact*contact)
	{
		HandleContact(contact, false);
	}

	void PhysicsContactListener::HandleContact(b2Contact*contact, bool begin)
	{
		// get collided bodies
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();

		// check if they are triggers
		bool sensorA = fixtureA->IsSensor();
		bool sensorB = fixtureB->IsSensor();

		// get entities
		auto bodyDataA = reinterpret_cast<BodyUserData*>(bodyA->GetUserData().pointer);
		auto bodyDataB = reinterpret_cast<BodyUserData*>(bodyB->GetUserData().pointer);

		entt::entity entityA = bodyDataA->attachedEntity;
		entt::entity entityB = bodyDataB->attachedEntity;

		// handle trigger or collision
		AddCollisionComponents(begin, sensorA, entityA, entityB);
		AddCollisionComponents(begin, sensorB, entityB, entityA);
	}

	void PhysicsContactListener::AddCollisionComponents(bool begin, bool sensor, entt::entity entityA, entt::entity entityB) const
	{
		// TODO:
		// need to create components for both scenarios

		if (sensor)
		{
			// is a trigger
			if (begin)
			{
				registry->emplace<TriggerEnterContactComponent>(entityA, entityB); // enter
			}
			else
			{
				registry->emplace<TriggerExitContactComponent>(entityA, entityB); // exit
			}
		}
		else
		{
			// involved in a collision
			if (begin)
			{
				registry->emplace<CollisionEnterContactComponent>(entityA, entityB); // enter
			}
			else
			{
				registry->emplace<CollisionExitContactComponent>(entityA, entityB); //exit
			}
		}
	}

} // Maize