#pragma once

#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "Maize/Math/Vector2.h"
#include "Maize/Math/Math.h"

namespace Maize {

	class PhysicsEngine;

	struct RaycastResult
	{
		float distance = 0; 				// distance from origin to point
		Vector2 point;       				// point of intersection in world coordinates
		Vector2 normal;      				// normal vector at the point of intersection
		float fraction = 0;     			// fraction along the ray where the intersection occurred
		entt::entity entity = entt::null; 	// entity that was hit
	};

	class Physics
	{
	public:
		static void Gravity(Vector2 gravity);
		static Vector2 Gravity();

		static RaycastResult Raycast(Vector2 origin, Vector2 direction, float distance = Math::Infinity(), uint16_t layer = 0x0001);
		static std::vector<RaycastResult> RaycastAll(Vector2 origin, Vector2 direction, float distance = Math::Infinity(), uint16_t layer = 0x0001);

		static entt::entity OverlapArea(Vector2 pointA, Vector2 pointB, uint16_t layer = 0x0001);
		static std::vector<entt::entity> OverlapAreaAll(Vector2 pointA, Vector2 pointB, uint16_t layer = 0x0001);

	private:
		friend class PhysicsEngine;

		struct RaycastCallback : public b2RayCastCallback
		{
			bool singleSearch = true;
			Vector2 origin;
			std::vector<RaycastResult> results;
			uint16_t layer = 0x0001; // default layer - temp

			RaycastCallback(bool singleSearch, Vector2 origin, uint16_t layer) : singleSearch(singleSearch), origin(origin), layer(layer)
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

		struct QueryCallBack: public b2QueryCallback
		{
			bool singleSearch = true;
			b2AABB boundingBox;
			std::vector<entt::entity> inside;
			uint16_t layer = 0x0001; // default layer - temp

			QueryCallBack(bool singleSearch, b2AABB boundingBox, uint16_t layer) : singleSearch(singleSearch), boundingBox(boundingBox), layer(layer)
			{

			}

			bool ReportFixture(b2Fixture *fixture) override
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

	 private:
		static void SetPhysicsEngine(b2World* engine);

	 private:
		inline static b2World* s_PhysicsEngine = nullptr;
	};

} // Maize
