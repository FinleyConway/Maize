#include "mpch.h"
#include "Maize/Scene/Systems/CollisionSystem.h"
#include "Maize/Physics/PhysicsEngine.h"
#include "Maize/Scene/Components.h"
#include "Maize/Math/Math.h"
#include "Maize/Math/Vector2.h"

namespace Maize {

	void CollisionSystem::Initialize(entt::registry& registry)
	{
		m_PhysicsEngine.Initialize({ 0.0f, -9.807f }, &m_ContactListener);

		auto view = registry.view<Transform, Rigidbody>();
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

			auto* userData = new BodyUserData();
			userData->attachedEntity = entity;

			// create body
			b2Body* body = m_PhysicsEngine.CreateBody(bProp, userData);
			rigidbody.body = body;

			// add a collider if that entity has this component
			if (registry.all_of<BoxCollider>(entity))
			{
				auto& boxCollider = registry.get<BoxCollider>(entity);

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
			if (registry.all_of<CircleCollider>(entity))
			{
				auto& circleCollider = registry.get<CircleCollider>(entity);

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
		}
	}

	void CollisionSystem::Update(entt::registry& reg, float deltaTime)
	{
		UpdateBox2d(reg);

		// update physics engine
		m_PhysicsEngine.Step(deltaTime);

		UpdateECS(reg);
	}

	void CollisionSystem::Shutdown(entt::registry& registry)
	{
		// free bodies
		auto view = registry.view<Transform, Rigidbody>();
		for (auto [entity, transform, rigidbody] : view.each())
		{
			auto body = rigidbody.body;
			delete reinterpret_cast<const BodyUserData*>(body->GetUserData().pointer);
		}

		m_PhysicsEngine.Shutdown();
	}

	void CollisionSystem::UpdateBox2d(entt::registry& reg)
	{
		auto view = reg.view<Transform, Rigidbody>();
		for (auto [entity, transform, rigidbody] : view.each())
		{
			const auto scale = Vector2(Math::Abs(transform.scale.x), Math::Abs(transform.scale.y));
			const float signY = Math::Sign(transform.scale.y);
			const float signX = Math::Sign(transform.scale.x);
			constexpr float minSize = 0.0001f;

			b2Body* body = rigidbody.body;

			body->SetType(static_cast<b2BodyType>(rigidbody.type));
			body->SetTransform({ transform.position.x, transform.position.y }, -transform.angle * Math::Deg2Rad());
			body->SetGravityScale(rigidbody.gravityScale);
			body->SetFixedRotation(rigidbody.fixedRotation);
			body->SetBullet(rigidbody.isContinuous);

			if (reg.all_of<BoxCollider>(entity))
			{
				auto& boxCollider = reg.get<BoxCollider>(entity);

				// cap min size
				if (boxCollider.size.x < minSize)
				{
					LOG_WARN("Cannot have x size smaller then {}", minSize);

					boxCollider.size.x = minSize;
				}
				if (boxCollider.size.y < minSize)
				{
					LOG_WARN("Cannot have y size smaller then {}", minSize);

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

			if (reg.all_of<CircleCollider>(entity))
			{
				auto& circleCollider = reg.get<CircleCollider>(entity);

				// cap min size
				if (circleCollider.radius < minSize)
				{
					LOG_WARN("Cannot have radius smaller the {}", minSize);

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
		}
	}

	void CollisionSystem::UpdateECS(entt::registry& reg)
	{
		// Update entities
		auto view = reg.view<Transform, Rigidbody>();
		for (auto [entity, transform, rigidbody] : view.each())
		{
			b2Body* body = rigidbody.body;
			auto position = body->GetPosition();

			transform.position = Vector2(position.x, position.y);
			transform.angle = Math::NormalizeAngle(-body->GetAngle() * Math::Rad2Deg());
		}
	}

} // Maize