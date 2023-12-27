#pragma once

#include <box2d/box2d.h>
#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Scene/Components.h"

namespace Maize {

	class CollisionSystem
	{
	public:
		void OnStart(ECS::EntityWorld& reg);
		void OnUpdate(ECS::EntityWorld& reg, float deltaTime);
		void OnDestroy();

	private:
		void UpdateBox2d(ECS::EntityWorld& reg)
		{
			for (auto entity : reg.GetEntityGroup<TransformComponent, RigidbodyComponent>())
			{
				const auto& [transform, rigidbody] = reg.GetComponents<TransformComponent, RigidbodyComponent>(entity);

				b2Body* body = rigidbody.body;

				body->SetType(static_cast<b2BodyType>(rigidbody.type));
				body->SetTransform({ transform.position.x, transform.position.y }, transform.angle);
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

					auto* polygonShape = static_cast<b2PolygonShape*>(body->GetFixtureList()->GetShape());
					polygonShape->SetAsBox((boxCollider.size.x / 2.0f) * transform.scale.x, (boxCollider.size.y / 2.0f) * transform.scale.y, { boxCollider.offset.x, boxCollider.offset.y }, 0.0f);

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

					auto* circleShape = static_cast<b2CircleShape*>(body->GetFixtureList()->GetShape());
					circleShape->m_p = { circleCollider.offset.x, circleCollider.offset.y };
					circleShape->m_radius = circleCollider.radius * std::max(transform.scale.x, transform.scale.y);

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

					float circleSize = (capsuleCollider.size.x / 2.0f) * transform.scale.x;
					float rectWidth = (capsuleCollider.size.x / 2.0f) * transform.scale.x;
					float rectHeight = ((capsuleCollider.size.y - capsuleCollider.size.x) / 2.0f) * transform.scale.y;

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
								circleShape->m_p = b2Vec2(capsuleCollider.offset.x, capsuleCollider.offset.y - rectHeight);
								current++;
							}
							// circle2
							else
							{
								circleShape->m_p = b2Vec2(capsuleCollider.offset.x, capsuleCollider.offset.y + rectHeight);
							}
						}
						// update rectangle
						else if (fixture->GetType() == b2Shape::e_polygon)
						{
							auto* rectangleShape = static_cast<b2PolygonShape*>(fixture->GetShape());
							rectangleShape->SetAsBox(rectWidth - 0.015f, rectHeight, { capsuleCollider.offset.x, capsuleCollider.offset.y }, 0.0f);
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

		b2World* m_PhysicsWorld;
	};

} // Maize
