#pragma once

#include <EntityComponentSystem/EntityWorld.h>
#include <Maize.h>

namespace Maize {

	struct RaycastResult
	{
		b2Fixture* collider = nullptr; 	// the collider that was hit
		float distance = 0; 			// distance from origin to point
		Vector2 point;       			// point of intersection in world coordinates
		Vector2 normal;      			// normal vector at the point of intersection
		float fraction = 0;     		// fraction along the ray where the intersection occurred
	};

	struct RaycastCallback : public b2RayCastCallback
	{
		RaycastResult result;
		Vector2 origin;
		uint16_t layer = 0x0001;

		RaycastCallback(Vector2 origin, uint16_t layer) : origin(origin), layer(layer)
		{
		}

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override
		{
			b2Filter filter = fixture->GetFilterData();

			if (filter.categoryBits & layer)
			{
				Vector2 hitPoint = Vector2(point.x, point.y);

				result.collider = fixture;
				result.distance = Vector2::Distance(hitPoint, origin);
				result.point = hitPoint;
				result.normal = Vector2(normal.x, normal.y);
				result.fraction = fraction;

				return fraction;
			}

			return -1.0f;
		}
	};

	RaycastResult Raycast(b2World* world, Vector2 origin, Vector2 direction, float distance = Math::Infinity(), uint16_t layer = 0x0001)
	{
		RaycastCallback callback(origin, layer);

		b2Vec2 from = b2Vec2(origin.x, origin.y);
		b2Vec2 to = from + distance * b2Vec2(direction.x, direction.y);

		// Perform the raycast
		world->RayCast(&callback, from, to);

		return callback.result;
	}

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			m_YellowPlayerIdle = Texture::Create("Assets/Players/Black/Gunner_Black_Idle.png");

			m_Camera = CreateCamera();

			for (float i = -50; i < 50; i += 1.0f)
			{
				auto ref = m_Reg.CreateEntity();
				auto& t = m_Reg.AddComponent<TransformComponent>(ref);
				t.position.x = i;
				m_Reg.AddComponent<RigidbodyComponent>(ref);
				auto& c = m_Reg.AddComponent<BoxColliderComponent>(ref);
			}

			m_Player = m_Reg.CreateEntity();
			auto& transform1 = m_Reg.AddComponent<TransformComponent>(m_Player);
			transform1.position.y = 2;

			auto& sprite = m_Reg.AddComponent<SpriteComponent>(m_Player);
			sprite.sprite = Sprite(*m_YellowPlayerIdle, sf::IntRect(0, 0, 48, 48), sf::Vector2f(24, 24), 48);

			auto& rb1 = m_Reg.AddComponent<RigidbodyComponent>(m_Player);
			rb1.type = RigidbodyComponent::BodyType::Dynamic;
			rb1.gravityScale = 1;
			rb1.fixedRotation = true;
			auto& col1 = m_Reg.AddComponent<CapsuleColliderComponent>(m_Player);
			col1.offset = Vector2(-0.04174951, 0.02186877);
			col1.size = Vector2(0.2962226, 0.6660039);
			col1.categoryBits = 0x0002;

			m_Collision.OnStart(m_Reg);
		}

		void OnDetach() override
		{
			m_Collision.OnDestroy();
		}

		void OnUpdate(float deltaTime) override
		{
			const auto& [t, r, col] = m_Reg.GetComponents<TransformComponent, RigidbodyComponent, CapsuleColliderComponent>(m_Player);
			const auto& [t1, c] = m_Reg.GetComponents<TransformComponent, CameraComponent>(m_Camera);

			Vector2 movement;

			// Horizontal Movement
			if (Input::IsKeyPressed(KeyCode::A))
			{
				movement.x = -1.0f;
				t.scale.x = -1;
			}
			if (Input::IsKeyPressed(KeyCode::D))
			{
				movement.x = 1.0f;
				t.scale.x = 1;
			}

			// Jumping
			float jumpHeight = 1;
			float jumpForce = Math::Sqrt(jumpHeight * -2.0f * m_Collision.m_PhysicsWorld->GetGravity().y);
			Vector2 origin(t.position.x, t.position.y - 0.3f);

			bool isGrounded = IsGrounded(origin);

			if (Input::IsKeyPressed(KeyCode::Space) && isGrounded)
			{
				r.body->SetLinearVelocity({ r.body->GetLinearVelocity().x, jumpForce });
			}

			// Apply horizontal force with clamping
			float maxSpeed = 3; // Adjust as needed
			float desiredSpeed = movement.x * 3;
			float currentSpeed = r.body->GetLinearVelocity().x;
			float force = desiredSpeed - currentSpeed;

			// Apply force with clamping
			if (force > 0)
			{
				force = std::min(force, maxSpeed - currentSpeed);
			}
			else if (force < 0)
			{
				force = std::max(force, -maxSpeed - currentSpeed);
			}

			r.body->ApplyForceToCenter({ force, 0.0f }, true);


			t1.position = Vector2::Lerp(t1.position, t.position, deltaTime * 3.0f);

			// backend
			m_Collision.OnUpdate(m_Reg, deltaTime);
			m_LocalToWorld.OnUpdate(m_Reg, deltaTime);

			m_Animate.OnUpdate(m_Reg, deltaTime);

			m_CameraSystem.OnUpdate(m_Reg, deltaTime);
			RenderingSystem::OnRender(m_Reg);

			if (ImGui::Begin("Debug Info"))
			{
				if (ImGui::Button("Show Colliders")) RenderingSystem::drawDebug = !RenderingSystem::drawDebug;

				ImGui::Text("Entity: %zu", m_Player);

				ImGui::Text("Position: %1.5f, %1.5f", t.position.x, t.position.y);
				ImGui::Text("Rotation: %1.5f", t.angle);
				ImGui::Text("Scale: %1.5f, %1.5f", t.scale.x, t.scale.y);

				//ImGui::Text("Velocity: %1.5f, %1.5f", r.body->GetLinearVelocity().x, r.body->GetLinearVelocity().y);

				ImGui::Text("Draw Calls: %d", Renderer::GetDrawCall());
				ImGui::Text("Frame Time: %1.5f", deltaTime);

				ImGui::End();
			}
		}

	private:
		bool IsGrounded(Vector2 origin)
		{
			RaycastResult hit = Raycast(m_Collision.m_PhysicsWorld, origin, Vector2::Down(), 0.05f);

			if (hit.collider != nullptr)
			{
				return true;
			}

			return false;
		}

		ECS::Entity CreateCamera()
		{
			auto camera = m_Reg.CreateEntity();

			m_Reg.AddComponent<TransformComponent>(camera);
			m_Reg.AddComponent<CameraComponent>(camera);

			return camera;
		}

	private:
		ECS::EntityWorld m_Reg;

		// backend
		CollisionSystem m_Collision;
		LocalToWorldSystem m_LocalToWorld;
		AnimationSystem m_Animate;
		CameraSystem m_CameraSystem;

		std::shared_ptr<Texture> m_YellowPlayerIdle;

		ECS::Entity m_Camera;
		ECS::Entity m_Player;
	};

}
