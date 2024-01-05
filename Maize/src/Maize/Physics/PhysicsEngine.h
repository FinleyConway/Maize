#pragma once

#include <box2d/box2d.h>

#include "Maize/Math/Vector2.h"

namespace Maize {

	enum class BodyType { Static = 0, Kinematic, Dynamic };

	struct BodyProperties
	{
		Vector2 position;
		float angle = 0;
		BodyType type = BodyType::Static;
		float gravityScale = 0;
		bool fixedRotation = false;
		bool isContinuous = false;
	};

	struct ColliderProperties
	{
		float friction = 0;
		float bounciness = 0;
		float bouncinessThreshold = 0;

		float density = 0;
		bool isTrigger = false;
		b2Filter filter;
	};

	enum class CapsuleDirection { Vertical = 0, Horizontal };

	class PhysicsEngine
	{
	public:
		static void Initialize(Vector2 gravity = { 0.0f, -9.807f }, b2ContactListener* contactListener = nullptr);
		static void Step(float deltaTime);
		static void Shutdown();

		static void SetGravity(Vector2 gravity);
		static Vector2 GetGravity();

		static b2Body* CreateBody(const BodyProperties& bProp, void* userData);
		static void RemoveBody(b2Body* body);

		static void CreateBoxCollider(b2Body* body, Vector2& size, Vector2 scale, Vector2 offset, const ColliderProperties& cProp);
		static void CreateCircleCollider(b2Body* body, float& radius, Vector2 scale, Vector2 offset, const ColliderProperties& cProp);
		static void CreateCapsuleCollider(b2Body* body, Vector2& size, Vector2 scale, Vector2 offset, const ColliderProperties& cProp, CapsuleDirection direction = CapsuleDirection::Vertical);

	private:
		friend class Physics;

		inline static b2World* s_PhysicsWorld;
		inline const static float cs_MinColliderSize = 0.0001f;
	};

} // Maize
