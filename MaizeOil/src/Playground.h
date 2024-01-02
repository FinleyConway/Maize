#pragma once

#include <EntityComponentSystem/EntityWorld.h>
#include <Maize.h>

namespace Maize {

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
			rb1.type = BodyType::Dynamic;
			rb1.gravityScale = 1;
			rb1.fixedRotation = true;
			auto& col1 = m_Reg.AddComponent<CapsuleColliderComponent>(m_Player);
			col1.offset = Vector2(-0.04174951, 0.02186877);
			col1.size = Vector2(0.2962226, 0.6660039);
			col1.categoryBits = 0x0002;

			CollisionSystem::OnStart(m_Reg);
		}

		void OnDetach() override
		{
			CollisionSystem::OnDestroy();
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
			float jumpForce = Math::Sqrt(jumpHeight * -2.0f * PhysicsEngine::GetGravity().y);
			Vector2 origin(t.position.x, t.position.y - 0.3f);

			bool isGrounded = IsGrounded(origin);

			if (Input::IsKeyPressed(KeyCode::Space) && isGrounded)
			{
				r.body->SetLinearVelocity({ r.body->GetLinearVelocity().x, jumpForce });
			}

			float maxSpeed = 3;
			float desiredSpeed = movement.x * 3;
			float currentSpeed = r.body->GetLinearVelocity().x;
			float force = desiredSpeed - currentSpeed;

			if (force > 0)
			{
				force = std::min(force, maxSpeed - currentSpeed);
			}
			else if (force < 0)
			{
				force = std::max(force, -maxSpeed - currentSpeed);
			}

			r.body->ApplyForceToCenter({ force, 0.0f }, true);

			t1.position = Vector2::SmoothDamp(t1.position, t.position, ref, 0.3f, Math::Infinity(), deltaTime);

			// backend
			CollisionSystem::OnUpdate(m_Reg, deltaTime);
			m_LocalToWorld.OnUpdate(m_Reg, deltaTime);

			m_Animate.OnUpdate(m_Reg, deltaTime);

			m_CameraSystem.OnUpdate(m_Reg, deltaTime);
			RenderingSystem::OnRender(m_Reg);

			if (ImGui::Begin("Debug Info"))
			{
				if (ImGui::Button("Show Colliders")) RenderingSystem::drawDebug = !RenderingSystem::drawDebug;
				if (ImGui::Button("Toggle chaos mode")) m_ToggleChaos = true;

				ImGui::Text("Entity: %zu", m_Player);

				ImGui::Text("Position: %1.5f, %1.5f", t.position.x, t.position.y);
				ImGui::Text("Rotation: %1.5f", t.angle);
				ImGui::Text("Scale: %1.5f, %1.5f", t.scale.x, t.scale.y);
				ImGui::Text("Velocity: %1.5f, %1.5f", r.body->GetLinearVelocity().x, r.body->GetLinearVelocity().y);

				ImGui::Text("Draw Calls: %d", Renderer::GetDrawCall());
				ImGui::Text("Frame Time: %1.5f", deltaTime);

				ImGui::End();
			}
		}

		Vector2 ref;

	private:
		bool IsGrounded(Vector2 origin)
		{
			RaycastResult hit = Physics::Raycast(origin, Vector2::Down(), 0.05f, 0x0001);

			if (hit.entity != -1)
			{
				if (m_ToggleChaos)
				{
					// to cause absolute chaos
					const auto& [r, b] = m_Reg.GetComponents<RigidbodyComponent, BoxColliderComponent>(hit.entity);
					r.type = BodyType::Dynamic;
					r.fixedRotation = true;
				}

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
		bool m_ToggleChaos = false;

		ECS::EntityWorld m_Reg;

		// backend
		LocalToWorldSystem m_LocalToWorld;
		AnimationSystem m_Animate;
		CameraSystem m_CameraSystem;

		std::shared_ptr<Texture> m_YellowPlayerIdle;

		ECS::Entity m_Camera;
		ECS::Entity m_Player;
	};

}
