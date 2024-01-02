#include "mpch.h"
#include "Maize/Physics/Physics.h"

namespace Maize {

	void Physics::Gravity(Vector2 gravity)
	{
		PhysicsEngine::SetGravity(gravity);
	}

	Vector2 Physics::Gravity()
	{
		return PhysicsEngine::GetGravity();
	}

	RaycastResult Physics::Raycast(Vector2 origin, Vector2 direction, float distance, uint16_t layer)
	{
		RaycastCallback callback(origin, layer);

		b2Vec2 from = b2Vec2(origin.x, origin.y);
		b2Vec2 to = from + distance * b2Vec2(direction.x, direction.y);

		// perform the raycast
		PhysicsEngine::m_PhysicsWorld->RayCast(&callback, from, to);

		return callback.result;
	}

} // Maize