#include "mpch.h"
#include "Maize/Physics/PhysicsEngine.h"
#include "Maize/Utils/Physics.h"

#include "Maize/Utils/Math.h"

namespace Maize {

	void PhysicsEngine::Initialize(Vector2 gravity, b2ContactListener* contactListener)
	{
		MZ_ASSERT_CORE(contactListener != nullptr, "contact listener has not been assigned!");

		s_PhysicsWorld = new b2World({ gravity.x, gravity.y });
		s_PhysicsDebugDraw = new PhysicsDebugDraw();

		s_PhysicsWorld->SetContactListener(contactListener);
		s_PhysicsWorld->SetDebugDraw(s_PhysicsDebugDraw);
	}

	void PhysicsEngine::Step(float deltaTime)
	{
		constexpr int32_t velocityIterations = 6;
		constexpr int32_t positionIterations = 2;

		s_PhysicsWorld->Step(deltaTime, velocityIterations, positionIterations);
	}

	void PhysicsEngine::Shutdown()
	{
		delete s_PhysicsWorld;
		s_PhysicsWorld = nullptr;
	}

	b2Body* PhysicsEngine::CreateBody(const BodyProperties& bProp, void* userData)
	{
		b2BodyDef bodyDef;

		bodyDef.position = b2Vec2(bProp.position.x, bProp.position.y);
		bodyDef.angle = bProp.angle;
		bodyDef.type = static_cast<b2BodyType>(bProp.type);
		bodyDef.gravityScale = bProp.gravityScale;
		bodyDef.fixedRotation = bProp.fixedRotation;
		bodyDef.bullet = bProp.isContinuous;

		if (userData != nullptr)
		{
			bodyDef.userData.pointer = reinterpret_cast<uintptr_t>(userData);
		}

		return s_PhysicsWorld->CreateBody(&bodyDef);
	}

	void PhysicsEngine::RemoveBody(b2Body* body)
	{
		s_PhysicsWorld->DestroyBody(body);
	}

	void PhysicsEngine::CreateBoxCollider(b2Body*body, Vector2& size, Vector2 scale, Vector2 offset, const ColliderProperties& cProp)
	{
		const auto AbsScale = Vector2(Math::Abs(scale.x), Math::Abs(scale.y));
		const float signY = Math::Sign(scale.y);
		const float signX = Math::Sign(scale.x);

		// cap min size
		if (size.x <= s_MinColliderSize)
		{
			MZ_LOG_WARN("Cannot have x size smaller then {}", s_MinColliderSize);

			size.x = s_MinColliderSize;
		}
		if (size.y <= s_MinColliderSize)
		{
			MZ_LOG_WARN("Cannot have y size smaller then {}", s_MinColliderSize);

			size.y = s_MinColliderSize;
		}

		// create body shape
		b2PolygonShape boxShape;
		boxShape.SetAsBox((size.x / 2.0f) * AbsScale.x, (size.y / 2.0f) * AbsScale.y, { offset.x * signX, offset.y * signY }, 0.0f);

		// assign body shape properties
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &boxShape;
		fixtureDef.density = cProp.density;
		fixtureDef.friction = cProp.friction;
		fixtureDef.restitution = cProp.bounciness;
		fixtureDef.restitutionThreshold = cProp.bouncinessThreshold;
		fixtureDef.isSensor = cProp.isTrigger;
		fixtureDef.filter.categoryBits = cProp.filter.categoryBits;

		// add shape to body
		body->CreateFixture(&fixtureDef);
	}

	void PhysicsEngine::CreateCircleCollider(b2Body*body, float& radius, Vector2 scale, Vector2 offset, const ColliderProperties& cProp)
	{
		const auto AbsScale = Vector2(Math::Abs(scale.x), Math::Abs(scale.y));
		const float signY = Math::Sign(scale.y);
		const float signX = Math::Sign(scale.x);

		// cap min size
		if (radius < s_MinColliderSize)
		{
			MZ_LOG_WARN("Cannot have radius smaller the {}", s_MinColliderSize);

			radius = s_MinColliderSize;
		}

		// create body shape
		b2CircleShape circleShape;
		circleShape.m_p = { offset.x * signX, offset.y * signY };
		circleShape.m_radius = radius * Math::Max(AbsScale.x, AbsScale.y);

		// assign body shape properties
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.density = cProp.density;
		fixtureDef.friction = cProp.friction;
		fixtureDef.restitution = cProp.bounciness;
		fixtureDef.restitutionThreshold = cProp.bouncinessThreshold;
		fixtureDef.isSensor = cProp.isTrigger;
		fixtureDef.filter.categoryBits = cProp.filter.categoryBits;

		// add shape to body
		body->CreateFixture(&fixtureDef);
	}

	void PhysicsEngine::CreateCapsuleCollider(b2Body*body, Vector2& size, Vector2 scale, Vector2 offset, const ColliderProperties& cProp, CapsuleDirection direction)
	{
		const auto AbsScale = Vector2(Math::Abs(scale.x), Math::Abs(scale.y));
		const float signY = Math::Sign(scale.y);
		const float signX = Math::Sign(scale.x);
		constexpr float rectOffset = 0.015f;

		// cap min size
		if (size.x < s_MinColliderSize)
		{
			MZ_LOG_WARN("Cannot have x size smaller then {}", s_MinColliderSize);

			size.x = s_MinColliderSize;
		}
		if (size.y < s_MinColliderSize)
		{
			MZ_LOG_WARN("Cannot have y size smaller then {}", s_MinColliderSize);

			size.y = s_MinColliderSize;
		}

		// calculate the dimensions for circles and rectangle
		const float circleSize = (size.x / 2.0f) * AbsScale.x;
		const float rectWidth = (size.x / 2.0f) * AbsScale.x;
		const float rectHeight = ((size.y - size.x) / 2.0f) * AbsScale.y;

		// assign body shape properties
		// top
		b2CircleShape circleShape1;
		circleShape1.m_radius = circleSize;

		// bottom
		b2CircleShape circleShape2;
		circleShape2.m_radius = circleSize;

		// middle
		b2PolygonShape rectangleShape;

		// shape capsule based on the capsule direction
		if (direction == CapsuleDirection::Vertical)
		{
			circleShape1.m_p = { offset.x * signX, offset.y * signY - rectHeight };
			circleShape2.m_p = { offset.x * signX, offset.y * signY + rectHeight };

			rectangleShape.SetAsBox(rectWidth - rectOffset, rectHeight, { offset.x * signX, offset.y * signY }, 0.0f);
		}
		else // CapsuleDirection::Horizontal
		{
			circleShape1.m_p = { offset.x * signX - rectHeight, offset.y * signY };
			circleShape2.m_p = { offset.x * signX + rectHeight, offset.y * signY };

			rectangleShape.SetAsBox(rectHeight, rectWidth - rectOffset, { offset.x * signX, offset.y * signY }, 0.0f);
		}

		// assign body shape properties
		b2FixtureDef fixtureDef;
		fixtureDef.density = cProp.density;
		fixtureDef.friction = cProp.friction;
		fixtureDef.restitution = cProp.bounciness;
		fixtureDef.restitutionThreshold = cProp.bouncinessThreshold;
		fixtureDef.isSensor = cProp.isTrigger;
		fixtureDef.filter.categoryBits = cProp.filter.categoryBits;

		// add shapes to body
		fixtureDef.shape = &circleShape1;
		body->CreateFixture(&fixtureDef);

		fixtureDef.shape = &circleShape2;
		body->CreateFixture(&fixtureDef);

		fixtureDef.shape = &rectangleShape;
		body->CreateFixture(&fixtureDef);
	}

	void PhysicsEngine::DrawDebug(sf::RenderTarget& renderer)
	{
		if (!s_DrawDebug)
		{
			s_PhysicsDebugDraw->SetFlags(b2Draw::e_shapeBit);
			s_PhysicsDebugDraw->SetRenderTarget(renderer);
		}

		s_PhysicsWorld->DebugDraw();
	}

} // Maize