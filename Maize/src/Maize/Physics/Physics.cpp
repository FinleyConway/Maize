#include "mpch.h"
#include "Maize/Physics/Physics.h"
#include "Maize/Physics/PhysicsEngine.h"

namespace Maize {

	void Physics::Gravity(Vector2 gravity)
	{
		s_PhysicsEngine->SetGravity({ gravity.x, gravity.y });
	}

	Vector2 Physics::Gravity()
	{
		auto g = s_PhysicsEngine->GetGravity();
		return { g.x, g.y };
	}

	RaycastResult Physics::Raycast(Vector2 origin, Vector2 direction, float distance, uint16_t layer)
	{
		RaycastCallback callback(origin, layer);

		b2Vec2 from = b2Vec2(origin.x, origin.y);
		b2Vec2 to = from + distance * b2Vec2(direction.x, direction.y);

		// perform the raycast
		s_PhysicsEngine->RayCast(&callback, from, to);

		return callback.result;
	}

	void Physics::SetPhysicsEngine(b2World* engine)
	{
		s_PhysicsEngine = engine;
	}

} // Maize