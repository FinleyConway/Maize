#pragma once

#include <box2d/box2d.h>

#include "Maize/Utils/Vector2.h"
#include "Maize/Utils/Physics.h"
#include "Maize/Physics/PhysicsDebugDraw.h"

namespace Maize {

	enum class BodyType
	{
		Static = 0, Kinematic, Dynamic
	};
	enum class CapsuleDirection
	{
		Vertical = 0, Horizontal
	};

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

	class Renderer;

	class PhysicsEngine
	{
	 public:
		static void Initialize(Vector2 gravity = { 0.0f, -9.807f }, b2ContactListener* contactListener = nullptr);
		static void Step(float deltaTime);
		static void Shutdown();

		static b2Body* CreateBody(const BodyProperties& bProp, void* userData);
		static void RemoveBody(b2Body* body);

		static void CreateBoxCollider(b2Body* body, Vector2& size, Vector2 scale, Vector2 offset, const ColliderProperties& cProp);
		static void CreateCircleCollider(b2Body* body, float& radius, Vector2 scale, Vector2 offset, const ColliderProperties& cProp);
		static void CreateCapsuleCollider(b2Body* body, Vector2& size, Vector2 scale, Vector2 offset, const ColliderProperties& cProp, CapsuleDirection direction = CapsuleDirection::Vertical);

		static void DrawDebug(sf::RenderTarget& renderer);

	 private:
		friend class Physics;

		inline static b2World* s_PhysicsWorld = nullptr;
		inline static PhysicsDebugDraw* s_PhysicsDebugDraw = nullptr;

		inline static bool s_DrawDebug = false;
		static constexpr float s_MinColliderSize = 0.0001f;
	};

} // Maize
