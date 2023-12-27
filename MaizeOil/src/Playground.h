#pragma once

#include <EntityComponentSystem/EntityWorld.h>
#include <Maize.h>

#include "CameraShakeSystem.h"

namespace Maize {

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			m_YellowPlayerIdle = Texture::Create("Assets/Players/Black/Gunner_Black_Idle.png");

			CreateCamera();

			for (float i = -10; i < 10; i += 0.48f)
			{
				auto ref = m_Reg.CreateEntity();
				auto& t = m_Reg.AddComponent<TransformComponent>(ref);
				t.position.x = i;
				m_Reg.AddComponent<RigidbodyComponent>(ref);
				auto& c = m_Reg.AddComponent<BoxColliderComponent>(ref);
				c.size = sf::Vector2f(0.48f, 0.48f);
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
			col1.offset = sf::Vector2f(-0.04174951, 0.02186877);
			col1.size = sf::Vector2f(0.2962226, 0.6660039);

			m_Collision.OnStart(m_Reg);
		}

		void OnDetach() override
		{
			m_Collision.OnDestroy();
		}

		void OnUpdate(float deltaTime) override
		{
			const auto& [t, r, b] = m_Reg.GetComponents<TransformComponent, RigidbodyComponent, CapsuleColliderComponent>(m_Player);

			sf::Vector2f movement;
			float size = 1;

			if (Input::IsKeyPressed(KeyCode::W))
			{
				movement.y++;
			}
			if (Input::IsKeyPressed(KeyCode::S))
			{
				movement.y--;
			}
            if (Input::IsKeyPressed(KeyCode::A))
			{
				movement.x--;
			}
            if (Input::IsKeyPressed(KeyCode::D))
			{
				movement.x++;
			}

			if (Input::IsKeyPressed(KeyCode::Q))
			{
				size++;
			}
			if (Input::IsKeyPressed(KeyCode::E))
			{
				b.isTrigger = true;
			}

			t.scale.x = size;
			t.scale.y = size;

			r.body->ApplyForceToCenter({ movement.x * 2.0f, movement.y * 5.0f }, true);

			float maxVelocity = 2.0f;
			b2Vec2 velocity = r.body->GetLinearVelocity();
			float currentVelocity = velocity.Length();

			/*if (currentVelocity > maxVelocity)
			{
				velocity *= maxVelocity / currentVelocity;
				r.body->SetLinearVelocity(velocity);
			}*/

			// custom
			m_Shake.Update(m_Reg, deltaTime);

			// backend
			m_Collision.OnUpdate(m_Reg, deltaTime);
			m_LocalToWorld.OnUpdate(m_Reg, deltaTime);
			m_Animate.OnUpdate(m_Reg, deltaTime);
			m_CameraSystem.OnUpdate(m_Reg, deltaTime);
			RenderingSystem::OnRender(m_Reg);

			if (ImGui::Begin("Debug"))
			{
				if (ImGui::Button("Show Colliders")) RenderingSystem::drawDebug = !RenderingSystem::drawDebug;

				ImGui::Text("Entity: %zu", m_Player);

				ImGui::Text("Position: %1.5f, %1.5f", t.position.x, t.position.y);
				ImGui::Text("Rotation: %1.5f", t.angle);

				ImGui::Text("Velocity: %1.5f, %1.5f", r.body->GetLinearVelocity().x, r.body->GetLinearVelocity().y);

				ImGui::Text("Draw Calls: %d", Renderer::GetDrawCall());

				ImGui::End();
			}
		}

	private:
		ECS::Entity CreateCamera()
		{
			auto camera = m_Reg.CreateEntity();

			m_Reg.AddComponent<TransformComponent>(camera);
			m_Reg.AddComponent<CameraComponent>(camera);

			return camera;
		}

	private:
		ECS::EntityWorld m_Reg;

		// custom
		CameraShakeSystem m_Shake;

		// backend
		CollisionSystem m_Collision;
		LocalToWorldSystem m_LocalToWorld;
		AnimationSystem m_Animate;
		CameraSystem m_CameraSystem;

		std::shared_ptr<Texture> m_YellowPlayerIdle;

		ECS::Entity m_Player;
	};

}
