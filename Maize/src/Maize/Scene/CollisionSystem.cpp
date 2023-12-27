#include "mpch.h"
#include "Maize/Scene/CollisionSystem.h"

namespace Maize {

	void CollisionSystem::OnStart(ECS::EntityWorld& reg)
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.807f });

		for (auto entity : reg.GetEntityGroup<TransformComponent, RigidbodyComponent>())
		{
			const auto& [transform, rigidbody] = reg.GetComponents<TransformComponent, RigidbodyComponent>(entity);

			b2BodyDef bodyDef;
			bodyDef.type = static_cast<b2BodyType>(rigidbody.type);
			bodyDef.position.Set(transform.position.x, transform.position.y);
			bodyDef.angle = transform.angle;
			bodyDef.gravityScale = rigidbody.gravityScale;
			bodyDef.fixedRotation = rigidbody.fixedRotation;
			if (rigidbody.detectionMode == RigidbodyComponent::CollisionDetection::Continuous)
			{
				bodyDef.bullet = true;
			}

			b2Body* body = m_PhysicsWorld->CreateBody(&bodyDef);
			rigidbody.body = body;

			if (reg.HasComponent<BoxColliderComponent>(entity))
			{
				auto& boxCollider = reg.GetComponent<BoxColliderComponent>(entity);

				b2PolygonShape boxShape;
				boxShape.SetAsBox((boxCollider.size.x / 2.0f) * transform.scale.x, (boxCollider.size.y / 2.0f) * transform.scale.y, { boxCollider.offset.x, boxCollider.offset.y }, 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxCollider.density;
				fixtureDef.friction = boxCollider.friction;
				fixtureDef.restitution = boxCollider.restitution;
				fixtureDef.restitutionThreshold = boxCollider.restitutionThreshold;
				fixtureDef.isSensor = boxCollider.isTrigger;
				body->CreateFixture(&fixtureDef);
			}

			if (reg.HasComponent<CircleColliderComponent>(entity))
			{
				auto& circleCollider = reg.GetComponent<CircleColliderComponent>(entity);

				b2CircleShape circleShape;
				circleShape.m_p = { circleCollider.offset.x, circleCollider.offset.y };
				circleShape.m_radius = circleCollider.radius * std::max(transform.scale.x, transform.scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = circleCollider.density;
				fixtureDef.friction = circleCollider.friction;
				fixtureDef.restitution = circleCollider.restitution;
				fixtureDef.restitutionThreshold = circleCollider.restitutionThreshold;
				fixtureDef.isSensor = circleCollider.isTrigger;
				body->CreateFixture(&fixtureDef);
			}

			if (reg.HasComponent<CapsuleColliderComponent>(entity))
			{
				auto& capsuleCollider = reg.GetComponent<CapsuleColliderComponent>(entity);

				// calculate the dimensions for circles and rectangle
				float circleSize = (capsuleCollider.size.x / 2.0f) * transform.scale.x;
				float rectWidth = (capsuleCollider.size.x / 2.0f) * transform.scale.x;
				float rectHeight = ((capsuleCollider.size.y - capsuleCollider.size.x) / 2.0f) * transform.scale.y;

				b2CircleShape circleShape1;
				circleShape1.m_radius = circleSize;
				circleShape1.m_p = b2Vec2(capsuleCollider.offset.x, capsuleCollider.offset.y - rectHeight);

				b2CircleShape circleShape2;
				circleShape2.m_radius = circleSize;
				circleShape2.m_p = b2Vec2(capsuleCollider.offset.x, capsuleCollider.offset.y + rectHeight);

				b2PolygonShape rectangleShape;
				rectangleShape.SetAsBox(rectWidth - 0.015f, rectHeight, { capsuleCollider.offset.x, capsuleCollider.offset.y }, 0.0f); // provided a very small offset to prevent the edges to collide with other bodies

				b2FixtureDef fixtureDef;
				fixtureDef.density = capsuleCollider.density;
				fixtureDef.friction = capsuleCollider.friction;
				fixtureDef.restitution = capsuleCollider.restitution;
				fixtureDef.restitutionThreshold = capsuleCollider.restitutionThreshold;
				fixtureDef.isSensor = capsuleCollider.isTrigger;
				fixtureDef.shape = &circleShape1;

				body->CreateFixture(&fixtureDef);

				fixtureDef.shape = &circleShape2;
				body->CreateFixture(&fixtureDef);

				fixtureDef.shape = &rectangleShape;
				body->CreateFixture(&fixtureDef);
			}
		}
	}

	void CollisionSystem::OnUpdate(ECS::EntityWorld& reg, float deltaTime)
	{
		UpdateBox2d(reg);

		const int32_t velocityIterations = 6;
		const int32_t positionIterations = 2;
		m_PhysicsWorld->Step(deltaTime, velocityIterations, positionIterations);

		for (auto entity : reg.GetEntityGroup<TransformComponent, RigidbodyComponent>())
		{
			const auto& [transform, rigidbody] = reg.GetComponents<TransformComponent, RigidbodyComponent>(entity);

			b2Body* body = rigidbody.body;
			auto position = body->GetPosition();

			transform.position = sf::Vector2f(position.x, position.y);
			transform.angle = -body->GetAngle() * 180.0f / 3.14f; // rad to deg
		}
	}

	void CollisionSystem::OnDestroy()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

} // Maize