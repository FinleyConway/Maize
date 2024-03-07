#pragma once

#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "Maize/Math/Vector2.h"
#include "Maize/Math/Math.h"

namespace Maize {

	class PhysicsEngine;

	struct RaycastResult
	{
		float distance = 0; 			// distance from origin to point
		Vector2 point;       			// point of intersection in world coordinates
		Vector2 normal;      			// normal vector at the point of intersection
		float fraction = 0;     		// fraction along the ray where the intersection occurred
	};

	class Physics
	{
	public:
		static void Gravity(Vector2 gravity);
		static Vector2 Gravity();

		static RaycastResult Raycast(Vector2 origin, Vector2 direction, float distance = Math::Infinity(), uint16_t layer = 0x0001);

	private:
		friend class PhysicsEngine;

		struct RaycastCallback : public b2RayCastCallback
		{
			RaycastResult result;
			Vector2 origin;
			uint16_t layer = 0x0001; // default layer - temp

			RaycastCallback(Vector2 origin, uint16_t layer) : origin(origin), layer(layer)
			{
			}

			float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
			{
				b2Filter filter = fixture->GetFilterData();

				// filter check to prevent unwanted objects to be hit by raycast
				if (filter.categoryBits & layer)
				{
					Vector2 hitPoint = Vector2(point.x, point.y);

					result.distance = Vector2::Distance(hitPoint, origin);
					result.point = hitPoint;
					result.normal = Vector2(normal.x, normal.y);
					result.fraction = fraction;

					return fraction;
				}

				// filter fixture
				return -1.0f;
			}
		};

	 private:
		static void SetPhysicsEngine(b2World* engine);

	 private:
		inline static b2World* s_PhysicsEngine = nullptr;
	};

} // Maize
