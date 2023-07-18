#include "Scene/CollisionContactListener.h"

namespace Maize {

	void CollisionContactListener::BeginContact(b2Contact* contact)
	{
		// get the fixtures involved in the contact
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		// get the bodies associated with the fixtures
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();

		// get entity associated with the bodies
		ECS::Entity entityA = bodyA->GetUserData().pointer;
		ECS::Entity entityB = bodyB->GetUserData().pointer;

		// set entity A and B component data
		auto& entityAData = m_Registry.AddComponent<CollisionContactComponent>(entityA);
		entityAData.collideEvent = CollisionContactComponent::CollidingEvent::Enter;
		entityAData.body = bodyA;
		entityAData.entity = entityB;

		auto& entityBData = m_Registry.AddComponent<CollisionContactComponent>(entityB);
		entityBData.collideEvent = CollisionContactComponent::CollidingEvent::Enter;
		entityBData.body = bodyB;
		entityBData.entity = entityA;
	}

	void CollisionContactListener::EndContact(b2Contact* contact)
	{
		// get the fixtures involved in the contact
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

		// get the bodies associated with the fixtures
		b2Body* bodyA = fixtureA->GetBody();
		b2Body* bodyB = fixtureB->GetBody();

		// get entity associated with the bodies
		ECS::Entity entityA = bodyA->GetUserData().pointer;
		ECS::Entity entityB = bodyB->GetUserData().pointer;

		// set entity A and B component data
		auto& entityAData = m_Registry.AddComponent<CollisionContactComponent>(entityA);
		entityAData.collideEvent = CollisionContactComponent::CollidingEvent::Exit;
		entityAData.body = bodyA;
		entityAData.entity = entityB;

		auto& entityBData = m_Registry.AddComponent<CollisionContactComponent>(entityB);
		entityBData.collideEvent = CollisionContactComponent::CollidingEvent::Exit;
		entityBData.body = bodyB;
		entityBData.entity = entityA;
	}

}