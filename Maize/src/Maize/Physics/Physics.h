#pragma once

#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "Maize/Math/Vector2.h"
#include "Maize/Math/Math.h"

namespace Maize {

	struct RaycastResult
	{
		float distance = 0;                // distance from origin to point
		Vector2 point;                    // point of intersection in world coordinates
		Vector2 normal;                    // normal vector at the point of intersection
		float fraction = 0;                // fraction along the ray where the intersection occurred
		entt::entity entity = entt::null;    // entity that was hit
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
	};

} // Maize
