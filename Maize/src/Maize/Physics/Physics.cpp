#include "mpch.h"
#include "Maize/Physics/Physics.h"
#include "Maize/Physics/PhysicsEngine.h"

namespace Maize {

	struct RaycastCallback : public b2RayCastCallback
	{
		bool singleSearch = true;
		Vector2 origin;
		std::vector<RaycastResult> results;
		uint16_t layer = 0x0001; // default layer - temp

		RaycastCallback(bool singleSearch, Vector2 origin, uint16_t layer)
			: singleSearch(singleSearch), origin(origin), layer(layer)
		{
		}

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			b2Filter filter = fixture->GetFilterData();

			// filter check to prevent unwanted objects to be hit by raycast
			if (filter.categoryBits & layer)
			{
				Vector2 hitPoint = Vector2(point.x, point.y);

				RaycastResult result;
				result.distance = Vector2::Distance(hitPoint, origin);
				result.point = hitPoint;
				result.normal = Vector2(normal.x, normal.y);
				result.fraction = fraction;
				result.entity = static_cast<entt::entity>(fixture->GetBody()->GetUserData().pointer);

				results.push_back(result);

				return !singleSearch; // 0/1 - terminate/continue
			}

			return false; // 0 - terminate
		}
	};

	struct QueryCallBack : public b2QueryCallback
	{
		bool singleSearch = true;
		b2AABB boundingBox;
		std::vector<entt::entity> inside;
		uint16_t layer = 0x0001; // default layer - temp

		QueryCallBack(bool singleSearch, b2AABB boundingBox, uint16_t layer)
			: singleSearch(singleSearch), boundingBox(boundingBox), layer(layer)
		{

		}

		bool ReportFixture(b2Fixture* fixture) override
		{
			b2Filter filter = fixture->GetFilterData();

			// filter check to prevent unwanted objects to be hit by raycast
			if (filter.categoryBits & layer)
			{
				if (boundingBox.Contains(fixture->GetAABB(0)))
				{
					inside.push_back(static_cast<entt::entity>(fixture->GetBody()->GetUserData().pointer));

					return !singleSearch;
				}
			}

			return false;
		}
	};

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