#include "mpch.h"
#include "Maize/Physics/PhysicsEngine.h"

namespace Maize {

	void PhysicsEngine::Initialize(Vector2 gravity)
	{
		m_PhysicsWorld = new b2World({ gravity.x, gravity.y });
	}

	void PhysicsEngine::Step(float deltaTime)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(deltaTime, velocityIterations, positionIterations);
	}

	void PhysicsEngine::Shutdown()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void PhysicsEngine::SetGravity(Vector2 gravity) { m_PhysicsWorld->SetGravity({ gravity.x, gravity.y }); }

	Vector2 PhysicsEngine::GetGravity() { return { m_PhysicsWorld->GetGravity().x, m_PhysicsWorld->GetGravity().y }; }

	b2Body* PhysicsEngine::CreateBody(const BodyProperties& bProp)
	{
		b2BodyDef bodyDef;

		bodyDef.position = b2Vec2(bProp.position.x, bProp.position.y);
		bodyDef.angle = bProp.angle;
		bodyDef.type = static_cast<b2BodyType>(bProp.type);
		bodyDef.gravityScale = bProp.gravityScale;
		bodyDef.fixedRotation = bProp.fixedRotation;
		bodyDef.bullet = bProp.isContinuous;

		bodyDef.userData.pointer = bProp.entity;

		return m_PhysicsWorld->CreateBody(&bodyDef);
	}

	void PhysicsEngine::RemoveBody(b2Body*body)
	{
		m_PhysicsWorld->DestroyBody(body);
	}

	void PhysicsEngine::CreateBoxCollider(b2Body*body, Vector2& size, Vector2 scale, Vector2 offset, const ColliderProperties& cProp)
	{
		const Vector2 AbsScale = Vector2(Math::Abs(scale.x), Math::Abs(scale.y));
		const float signY = Math::Sign(scale.y);
		const float signX = Math::Sign(scale.x);

		// TODO:
		// add warnings when this happens
		// cap min size
		if (size.x <= cs_MinColliderSize)
		{
			size.x = cs_MinColliderSize;
		}
		if (size.y <= cs_MinColliderSize)
		{
			size.y = cs_MinColliderSize;
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
		const Vector2 AbsScale = Vector2(Math::Abs(scale.x), Math::Abs(scale.y));
		const float signY = Math::Sign(scale.y);
		const float signX = Math::Sign(scale.x);

		// TODO:
		// add warnings when this happens
		// cap min size
		if (radius <= cs_MinColliderSize)
		{
			radius = cs_MinColliderSize;
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
		const Vector2 AbsScale = Vector2(Math::Abs(scale.x), Math::Abs(scale.y));
		const float signY = Math::Sign(scale.y);
		const float signX = Math::Sign(scale.x);
		const float rectOffset = 0.015f;

		// TODO:
		// add warnings when this happens
		// cap min size
		if (size.x <= cs_MinColliderSize)
		{
			size.x = cs_MinColliderSize;
		}
		if (size.y <= cs_MinColliderSize)
		{
			size.y = cs_MinColliderSize;
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

} // Maize