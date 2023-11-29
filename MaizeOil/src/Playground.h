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
			m_YellowPlayerIdle = Texture::Create("Assets/Players/Yellow/Gunner_Yellow_Idle.png");
			m_YellowPlayerRun = Texture::Create("Assets/Players/Yellow/Gunner_Yellow_Run.png");

			CreateCamera();

			m_Player = m_Reg.CreateEntity();
			auto gun = m_Reg.CreateEntity();

			SetupParentChildRelation(m_Player, gun);

			m_Reg.GetComponent<LocalTransformComponent>(gun).position = sf::Vector2f(20, 20);

			m_Reg.AddComponent<SpriteComponent>(m_Player).sprite = Sprite(*m_YellowPlayerIdle, sf::IntRect(0, 0, 48, 48), sf::Vector2f(48, 48) / 2.0f);
			m_Reg.AddComponent<SpriteComponent>(gun).sprite = Sprite(*m_YellowPlayerRun);

			auto& animator = m_Reg.AddComponent<AnimatorComponent>(m_Player);
			animator.currentState = "Idle";
			for (int32_t i = 0 ; i < 5; i++)
				animator.states["Idle"].AddFrame(i, sf::IntRect(48 * i, 0, 48, 48), 0.1f);
			for (int32_t i = 0 ; i < 5; i++)
				animator.states["Run"].AddFrame(i, sf::IntRect(48 * i, 0, 48, 48), 0.1f);
		}

		void OnUpdate(float deltaTime) override
		{
			auto [transform, sprite, animator] = m_Reg.GetComponents<TransformComponent, SpriteComponent, AnimatorComponent>(m_Player);
			sf::Vector2f movement;

			if (Input::IsKeyPressed(KeyCode::W))
			{
				movement.y--;
			}
            if (Input::IsKeyPressed(KeyCode::A))
			{
				movement.x--;
				transform.scale.x = -1;
			}
            if (Input::IsKeyPressed(KeyCode::S))
			{
				movement.y++;
			}
            if (Input::IsKeyPressed(KeyCode::D))
			{
				movement.x++;
				transform.scale.x = 1;
			}

			if (movement != sf::Vector2f(0,0))
			{
				animator.currentState = "Run";
				sprite.sprite.SetTexture(*m_YellowPlayerRun);
			}
			else
			{
				animator.currentState = "Idle";
				sprite.sprite.SetTexture(*m_YellowPlayerIdle);
			}

			transform.position += movement * deltaTime * 100.0f;


			// custom
			m_Shake.Update(m_Reg, deltaTime);

			// backend
			m_LocalToWorld.OnUpdate(m_Reg, deltaTime);
			m_Animate.OnUpdate(m_Reg, deltaTime);
			m_CameraSystem.OnUpdate(m_Reg, deltaTime);
			m_Render.OnRender(m_Reg);
		}

	private:
		ECS::Entity CreateCamera()
		{
			auto camera = m_Reg.CreateEntity();

			m_Reg.AddComponent<TransformComponent>(camera);
			m_Reg.AddComponent<CameraComponent>(camera);

			return camera;
		}

		void SetupParentChildRelation(ECS::Entity parentEntity, ECS::Entity childEntity)
		{
			m_Reg.AddComponent<TransformComponent>(parentEntity);
			auto& parent = m_Reg.AddComponent<ParentComponent>(parentEntity);

			m_Reg.AddComponent<TransformComponent>(childEntity);
			m_Reg.AddComponent<LocalTransformComponent>(childEntity);
			auto& child = m_Reg.AddComponent<ChildComponent>(childEntity);

			parent.children.push_back(childEntity);
			child.parent = parentEntity;
		}

	private:
		ECS::EntityWorld m_Reg;

		// custom
		CameraShakeSystem m_Shake;

		// backend
		LocalToWorldSystem m_LocalToWorld;
		AnimationSystem m_Animate;
		CameraSystem m_CameraSystem;
		RenderingSystem m_Render;

		std::shared_ptr<Texture> m_YellowPlayerIdle;
		std::shared_ptr<Texture> m_YellowPlayerRun;

		ECS::Entity m_Player;
	};

}
