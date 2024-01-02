#include "mpch.h"
#include "Maize/Scene/CollisionSystem.h"
#include "Maize/Math/Math.h"

namespace Maize {

	void CollisionSystem::OnStart(ECS::EntityWorld& reg)
	{
		m_PhysicsWorld = new b2World({ 0.0f, -9.807f });

		for (auto entity : reg.GetEntityGroup<TransformComponent, RigidbodyComponent>())
		{
			const auto& [transform, rigidbody] = reg.GetComponents<TransformComponent, RigidbodyComponent>(entity);

			const sf::Vector2f scale = sf::Vector2f(Math::Abs(transform.scale.x), Math::Abs(transform.scale.y));
			const float signY = Math::Sign(transform.scale.y);
			const float signX = Math::Sign(transform.scale.x);
			const float minSize = 0.0001f;

			b2BodyDef bodyDef;
			bodyDef.type = static_cast<b2BodyType>(rigidbody.type);
			bodyDef.position.Set(transform.position.x, transform.position.y);
			bodyDef.angle = transform.angle * Math::Deg2Rad();
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

				// TODO:
				// add warnings when this happens
				if (boxCollider.size.x <= minSize)
				{
					boxCollider.size.x = minSize;
				}
				if (boxCollider.size.y <= minSize)
				{
					boxCollider.size.y = minSize;
				}

				b2PolygonShape boxShape;
				boxShape.SetAsBox((boxCollider.size.x / 2.0f) * scale.x, (boxCollider.size.y / 2.0f) * scale.y, { boxCollider.offset.x * signX, boxCollider.offset.y * signY }, 0.0f);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &boxShape;
				fixtureDef.density = boxCollider.density;
				fixtureDef.friction = boxCollider.friction;
				fixtureDef.restitution = boxCollider.restitution;
				fixtureDef.restitutionThreshold = boxCollider.restitutionThreshold;
				fixtureDef.isSensor = boxCollider.isTrigger;
				fixtureDef.filter.categoryBits = boxCollider.categoryBits;
				body->CreateFixture(&fixtureDef);
			}

			if (reg.HasComponent<CircleColliderComponent>(entity))
			{
				auto& circleCollider = reg.GetComponent<CircleColliderComponent>(entity);

				// TODO:
				// add warnings when this happens
				if (circleCollider.radius <= minSize)
				{
					circleCollider.radius = minSize;
				}

				b2CircleShape circleShape;
				circleShape.m_p = { circleCollider.offset.x * signX, circleCollider.offset.y * signY };
				circleShape.m_radius = circleCollider.radius * Math::Max(scale.x, scale.y);

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circleShape;
				fixtureDef.density = circleCollider.density;
				fixtureDef.friction = circleCollider.friction;
				fixtureDef.restitution = circleCollider.restitution;
				fixtureDef.restitutionThreshold = circleCollider.restitutionThreshold;
				fixtureDef.isSensor = circleCollider.isTrigger;
				fixtureDef.filter.categoryBits = circleCollider.categoryBits;
				body->CreateFixture(&fixtureDef);
			}

			if (reg.HasComponent<CapsuleColliderComponent>(entity))
			{
				auto& capsuleCollider = reg.GetComponent<CapsuleColliderComponent>(entity);

				// TODO:
				// add warnings when this happens
				if (capsuleCollider.size.x <= minSize)
				{
					capsuleCollider.size.x = minSize;
				}
				if (capsuleCollider.size.y <= minSize)
				{
					capsuleCollider.size.y = minSize;
				}

				// calculate the dimensions for circles and rectangle
				const float circleSize = (capsuleCollider.size.x / 2.0f) * scale.x;
				const float rectWidth = (capsuleCollider.size.x / 2.0f) * scale.x;
				const float rectHeight = ((capsuleCollider.size.y - capsuleCollider.size.x) / 2.0f) * scale.y;

				b2CircleShape circleShape1;
				circleShape1.m_radius = circleSize;
				circleShape1.m_p = { capsuleCollider.offset.x * signX, capsuleCollider.offset.y * signY - rectHeight };

				b2CircleShape circleShape2;
				circleShape2.m_radius = circleSize;
				circleShape2.m_p = { capsuleCollider.offset.x * signX, capsuleCollider.offset.y * signY + rectHeight };

				b2PolygonShape rectangleShape;
				rectangleShape.SetAsBox(rectWidth - 0.015f, rectHeight, { capsuleCollider.offset.x * signX, capsuleCollider.offset.y * signY }, 0.0f); // provided a very small offset to prevent the edges to collide with other bodies

				b2FixtureDef fixtureDef;
				fixtureDef.density = capsuleCollider.density;
				fixtureDef.friction = capsuleCollider.friction;
				fixtureDef.restitution = capsuleCollider.restitution;
				fixtureDef.restitutionThreshold = capsuleCollider.restitutionThreshold;
				fixtureDef.isSensor = capsuleCollider.isTrigger;
				fixtureDef.filter.categoryBits = capsuleCollider.categoryBits;
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

			transform.position = Vector2(position.x, position.y);
			transform.angle = NormalizeAngle(body->GetAngle() * Math::Rad2Deg());;
		}
	}

	void CollisionSystem::OnDestroy()
	{
		delete m_PhysicsWorld;
		m_PhysicsWorld = nullptr;
	}

	void CollisionSystem::UpdateBox2d(ECS::EntityWorld& reg)
	{
		for (auto entity : reg.GetEntityGroup<TransformComponent, RigidbodyComponent>())
		{
			const auto& [transform, rigidbody] = reg.GetComponents<TransformComponent, RigidbodyComponent>(entity);

			const sf::Vector2f scale = sf::Vector2f(Math::Abs(transform.scale.x), Math::Abs(transform.scale.y));
			const float signY = Math::Sign(transform.scale.y);
			const float signX = Math::Sign(transform.scale.x);
			const float minSize = 0.0001f;

			b2Body* body = rigidbody.body;

			body->SetType(static_cast<b2BodyType>(rigidbody.type));
			body->SetTransform({ transform.position.x, transform.position.y }, transform.angle * Math::Deg2Rad());
			body->SetGravityScale(rigidbody.gravityScale);
			body->SetFixedRotation(rigidbody.fixedRotation);
			if (rigidbody.detectionMode == RigidbodyComponent::CollisionDetection::Continuous)
			{
				body->SetBullet(true);
			}
			else
			{
				body->SetBullet(false);
			}

			if (reg.HasComponent<BoxColliderComponent>(entity))
			{
				auto& boxCollider = reg.GetComponent<BoxColliderComponent>(entity);

				// TODO:
				// add warnings when this happens
				if (boxCollider.size.x <= minSize)
				{
					boxCollider.size.x = minSize;
				}
				if (boxCollider.size.y <= minSize)
				{
					boxCollider.size.y = minSize;
				}

				auto* polygonShape = static_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
				polygonShape->SetAsBox((boxCollider.size.x / 2.0f) * scale.x, (boxCollider.size.y / 2.0f) * scale.y, { boxCollider.offset.x * signX, boxCollider.offset.y * signY }, 0.0f);

				b2Fixture* fixture = body->GetFixtureList();
				fixture->SetDensity(boxCollider.density);
				fixture->SetFriction(boxCollider.friction);
				fixture->SetRestitution(boxCollider.restitution);
				fixture->SetRestitutionThreshold(boxCollider.restitutionThreshold);
				fixture->SetSensor(boxCollider.isTrigger);
			}

			if (reg.HasComponent<CircleColliderComponent>(entity))
			{
				auto& circleCollider = reg.GetComponent<CircleColliderComponent>(entity);

				// TODO:
				// add warnings when this happens
				if (circleCollider.radius <= minSize)
				{
					circleCollider.radius = minSize;
				}

				auto* circleShape = static_cast<b2CircleShape*>(body->GetFixtureList()->GetShape());
				circleShape->m_p = { circleCollider.offset.x * signX, circleCollider.offset.y * signY };
				circleShape->m_radius = circleCollider.radius * Math::Max(scale.x, scale.y);

				b2Fixture* fixture = body->GetFixtureList();
				fixture->SetDensity(circleCollider.density);
				fixture->SetFriction(circleCollider.friction);
				fixture->SetRestitution(circleCollider.restitution);
				fixture->SetRestitutionThreshold(circleCollider.restitutionThreshold);
				fixture->SetSensor(circleCollider.isTrigger);
			}

			if (reg.HasComponent<CapsuleColliderComponent>(entity))
			{
				auto& capsuleCollider = reg.GetComponent<CapsuleColliderComponent>(entity);

				// TODO:
				// add warnings when this happens
				if (capsuleCollider.size.x <= minSize)
				{
					capsuleCollider.size.x = minSize;
				}
				if (capsuleCollider.size.y <= minSize)
				{
					capsuleCollider.size.y = minSize;
				}

				const float circleSize = (capsuleCollider.size.x / 2.0f) * scale.x;
				const float rectWidth = (capsuleCollider.size.x / 2.0f) * scale.x;
				const float rectHeight = ((capsuleCollider.size.y - capsuleCollider.size.x) / 2.0f) * scale.y;

				// loop through each segment of the capsule collider
				uint8_t current = 0;
				for (b2Fixture* fixture = body->GetFixtureList(); fixture; fixture = fixture->GetNext())
				{
					//  update circle
					if (fixture->GetType() == b2Shape::e_circle)
					{
						auto* circleShape = static_cast<b2CircleShape*>(fixture->GetShape());
						circleShape->m_radius = circleSize;

						// circle1
						if (current == 0)
						{
							circleShape->m_p = { capsuleCollider.offset.x * signX, capsuleCollider.offset.y * signY - rectHeight };
							current++;
						}
							// circle2
						else
						{
							circleShape->m_p = { capsuleCollider.offset.x * signX, capsuleCollider.offset.y * signY + rectHeight };
						}
					}
					// update rectangle
					else if (fixture->GetType() == b2Shape::e_polygon)
					{
						auto* rectangleShape = static_cast<b2PolygonShape*>(fixture->GetShape());
						rectangleShape->SetAsBox(rectWidth - 0.015f, rectHeight, { capsuleCollider.offset.x * signX, capsuleCollider.offset.y * signY }, 0.0f);
					}

					fixture->SetDensity(capsuleCollider.density);
					fixture->SetFriction(capsuleCollider.friction);
					fixture->SetRestitution(capsuleCollider.restitution);
					fixture->SetRestitutionThreshold(capsuleCollider.restitutionThreshold);
					fixture->SetSensor(capsuleCollider.isTrigger);
				}
			}
		}
	}

} // Maize