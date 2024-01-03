#include "mpch.h"
#include "CollisionSystem.h"
#include "Maize/Physics/PhysicsEngine.h"
#include "Maize/Scene/Components.h"
#include "Maize/Math/Math.h"
#include "Maize/Math/Vector2.h"

namespace Maize {

	void CollisionSystem::OnStart(entt::registry& reg)
	{
		PhysicsEngine::Initialize();

		auto view = reg.view<TransformComponent, RigidbodyComponent>();
		for (auto [entity, transform, rigidbody] : view.each())
		{
			// apply rigidbody properties
			BodyProperties bProp;
			bProp.position = transform.position;
			bProp.angle = transform.angle * Math::Deg2Rad();
			bProp.type = rigidbody.type;
			bProp.gravityScale = rigidbody.gravityScale;
			bProp.fixedRotation = rigidbody.fixedRotation;
			bProp.isContinuous = rigidbody.isContinuous;

			// create body
			b2Body* body = PhysicsEngine::CreateBody(bProp);
			rigidbody.body = body;

			// add a collider if that entity has this component
			if (reg.all_of<BoxColliderComponent>(entity))
			{
				auto& boxCollider = reg.get<BoxColliderComponent>(entity);

				// apply collider properties
				ColliderProperties cProp;
				cProp.friction = boxCollider.friction;
				cProp.bounciness = boxCollider.restitution;
				cProp.bouncinessThreshold = boxCollider.restitutionThreshold;

				cProp.density = boxCollider.density;
				cProp.isTrigger = boxCollider.isTrigger;
				cProp.filter.categoryBits = boxCollider.categoryBits;

				// create collider
				PhysicsEngine::CreateBoxCollider(body, boxCollider.size, transform.scale, boxCollider.offset, cProp);
			}

			// add a collider if that entity has this component
			if (reg.all_of<CircleColliderComponent>(entity))
			{
				auto& circleCollider = reg.get<CircleColliderComponent>(entity);

				// apply collider properties
				ColliderProperties cProp;
				cProp.friction = circleCollider.friction;
				cProp.bounciness = circleCollider.restitution;
				cProp.bouncinessThreshold = circleCollider.restitutionThreshold;

				cProp.density = circleCollider.density;
				cProp.isTrigger = circleCollider.isTrigger;
				cProp.filter.categoryBits = circleCollider.categoryBits;

				// create collider
				PhysicsEngine::CreateCircleCollider(body, circleCollider.radius, transform.scale, circleCollider.offset, cProp);
			}

			// add a collider if that entity has this component
			if (reg.all_of<CapsuleColliderComponent>(entity))
			{
				auto& capsuleCollider = reg.get<CapsuleColliderComponent>(entity);

				// apply collider properties
				ColliderProperties cProp;
				cProp.friction = capsuleCollider.friction;
				cProp.bounciness = capsuleCollider.restitution;
				cProp.bouncinessThreshold = capsuleCollider.restitutionThreshold;

				cProp.density = capsuleCollider.density;
				cProp.isTrigger = capsuleCollider.isTrigger;
				cProp.filter.categoryBits = capsuleCollider.categoryBits;

				// create collider
				PhysicsEngine::CreateCapsuleCollider(body, capsuleCollider.size, transform.scale, capsuleCollider.offset, cProp, capsuleCollider.direction);
			}
		}
	}

	void CollisionSystem::OnUpdate(entt::registry& reg, float deltaTime)
	{
		UpdateBox2d(reg);

		// update physics engine
		PhysicsEngine::Step(deltaTime);

		UpdateECS(reg);
	}

	void CollisionSystem::OnDestroy()
	{
		PhysicsEngine::Shutdown();
	}

	void CollisionSystem::UpdateBox2d(entt::registry& reg)
	{
		auto view = reg.view<TransformComponent, RigidbodyComponent>();
		for (auto [entity, transform, rigidbody] : view.each())
		{
			const Vector2 scale = Vector2(Math::Abs(transform.scale.x), Math::Abs(transform.scale.y));
			const float signY = Math::Sign(transform.scale.y);
			const float signX = Math::Sign(transform.scale.x);
			const float minSize = 0.0001f;

			b2Body* body = rigidbody.body;

			body->SetType(static_cast<b2BodyType>(rigidbody.type));
			body->SetTransform({ transform.position.x, transform.position.y }, transform.angle * Math::Deg2Rad());
			body->SetGravityScale(rigidbody.gravityScale);
			body->SetFixedRotation(rigidbody.fixedRotation);
			body->SetBullet(rigidbody.isContinuous);

			if (reg.all_of<BoxColliderComponent>(entity))
			{
				auto& boxCollider = reg.get<BoxColliderComponent>(entity);

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

			if (reg.all_of<CircleColliderComponent>(entity))
			{
				auto& circleCollider = reg.get<CircleColliderComponent>(entity);

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

			if (reg.all_of<CapsuleColliderComponent>(entity))
			{
				auto& capsuleCollider = reg.get<CapsuleColliderComponent>(entity);

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

	void CollisionSystem::UpdateECS(entt::registry& reg)
	{
		// update entities
		auto view = reg.view<TransformComponent, RigidbodyComponent>();
		for (auto [entity, transform, rigidbody] : view.each())
		{
			b2Body* body = rigidbody.body;
			auto position = body->GetPosition();

			transform.position = Vector2(position.x, position.y);
			transform.angle = NormalizeAngle(body->GetAngle() * Math::Rad2Deg());;
		}
	}

} // Maize