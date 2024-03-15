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
		RaycastCallback callback(true, origin, layer);

		b2Vec2 from = b2Vec2(origin.x, origin.y);
		b2Vec2 to = from + distance * b2Vec2(direction.x, direction.y);

		// perform the raycast
		s_PhysicsEngine->RayCast(&callback, from, to);

		if (!callback.results.empty())
		{
			return callback.results[0];
		}

		return {};
	}

	std::vector<RaycastResult> Physics::RaycastAll(Vector2 origin, Vector2 direction, float distance, uint16_t layer)
	{
		RaycastCallback callback(false, origin, layer);

		b2Vec2 from = b2Vec2(origin.x, origin.y);
		b2Vec2 to = from + distance * b2Vec2(direction.x, direction.y);

		// perform the raycast
		s_PhysicsEngine->RayCast(&callback, from, to);

		return callback.results;
	}

	entt::entity Physics::OverlapArea(Vector2 pointA, Vector2 pointB, uint16_t layer)
	{
		b2AABB aabb;
		aabb.upperBound = b2Vec2(pointA.x, pointA.y);
		aabb.lowerBound = b2Vec2(pointB.x, pointB.y);

		QueryCallBack callback(true, aabb, layer);

		s_PhysicsEngine->QueryAABB(&callback, aabb);

		if (!callback.inside.empty())
		{
			return callback.inside[0];
		}

		return entt::null;
	}

	std::vector<entt::entity> Physics::OverlapAreaAll(Vector2 pointA, Vector2 pointB, uint16_t layer)
	{
		b2AABB aabb;
		aabb.upperBound = b2Vec2(pointA.x, pointA.y);
		aabb.lowerBound = b2Vec2(pointB.x, pointB.y);

		QueryCallBack callback(false, aabb, layer);

		s_PhysicsEngine->QueryAABB(&callback, aabb);

		return callback.inside;
	}

	void Physics::SetPhysicsEngine(b2World* engine)
	{
		s_PhysicsEngine = engine;
	}

} // Maize