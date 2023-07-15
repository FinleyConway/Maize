#include "Scene/Systems/PhysicsSystem.h"

namespace Maize {

	PhysicsSystem::PhysicsSystem(PointF gravity)
	{
		m_World = new b2World({ gravity.x, gravity.y });
	}

	void PhysicsSystem::OnStart(ECS::EntityWorld& registry)
	{
		for (const auto& entity : registry.GetEntityGroup<TransformComponent, RigidbodyComponent>())
		{
			const auto& [transform, rigidbody] = registry.GetComponents<TransformComponent, RigidbodyComponent>(entity);

			b2BodyDef bodyDef;
			bodyDef.type = static_cast<b2BodyType>(rigidbody.type);
			bodyDef.position.Set(transform.position.x, transform.position.y);
			bodyDef.angle = transform.angle;

			b2Body* body = m_World->CreateBody(&bodyDef);
			body->SetFixedRotation(rigidbody.fixedRotation);
			rigidbody.body = body;

			if (registry.HasComponent<SquareCollider>(entity))
			{
				const auto& square = registry.GetComponent<SquareCollider>(entity);

				b2PolygonShape squareShape;
				squareShape.SetAsBox((square.size.x / 2) * transform.scale.x,
					(square.size.y / 2) * transform.scale.y,
					b2Vec2(square.offset.x, square.offset.y), 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &squareShape;
				fixtureDef.density = square.density;
				fixtureDef.friction = square.friction;
				fixtureDef.restitution = square.restitution;
				fixtureDef.restitutionThreshold = square.restitutionThreshold;
				body->CreateFixture(&fixtureDef);
			}

			if (registry.HasComponent<CircleCollider>(entity))
			{
				const auto& circle = registry.GetComponent<CircleCollider>(entity);

				b2CircleShape circleShape;
				circleShape.m_p.Set(circle.offset.x, circle.offset.y);
				circleShape.m_radius = transform.scale.x * circle.radius;

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = circle.density;
				fixtureDef.friction = circle.friction;
				fixtureDef.restitution = circle.restitution;
				fixtureDef.restitutionThreshold = circle.restitutionThreshold;
			}
		}
	}

	void PhysicsSystem::OnUpdate(ECS::EntityWorld& registry, float dt)
	{
		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;

		m_World->Step(dt, velocityIterations, positionIterations);

		for (const auto& entity : registry.GetEntityGroup<TransformComponent, RigidbodyComponent>())
		{
			const auto& [transform, rigidbody] = registry.GetComponents<TransformComponent, RigidbodyComponent>(entity);

			const b2Body* body = (b2Body*)rigidbody.body;
			const auto& position = body->GetPosition();

			transform.position.x = position.x;
			transform.position.y = position.y;
			transform.angle = body->GetAngle();
		}
	}

}