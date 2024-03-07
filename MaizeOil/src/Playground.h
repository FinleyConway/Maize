#pragma once

#include <Maize.h>

namespace Maize {

	class CustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float deltaTime) override
		{
			auto view = registry.view<Transform, SpriteRenderer>();
			for (auto [entity, transform, sprite] : view.each())
			{
				Vector2 dir;

				if (Input::GetButton(KeyCode::D))
				{
					dir.x += 1;
				}
				if (Input::GetButton(KeyCode::A))
				{
					dir.x -= 1;
				}

				transform.position += dir * deltaTime * 5;
			}

			if (Input::GetButtonDown(KeyCode::Space))
			{
				SceneManager::LoadScene("Default1");
			}
		}
	};

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			m_Tx = Texture("/home/finley/GameShiz/Sprites/PNG/Retina/station_C.png");
			m_Tx1 = Texture("/home/finley/GameShiz/Sprites/PNG/Retina/star_tiny.png");

			// scene 1
			auto scene1 = SceneManager::CreateScene("Default");
			auto initScene1 = [&](Scene& scene)
			{
				scene.AddSystem<CustomSystem>("Custom");

				auto e1 = scene.CreateEntity();
				auto& spriteR = e1.AddComponent<SpriteRenderer>();
				spriteR.sprite = Sprite(m_Tx, { 0, 0, 128, 128 }, { 128 / 2, 128 / 2});

				auto& rb = e1.AddComponent<Rigidbody>();
				rb.type = BodyType::Dynamic;
				auto& box = e1.AddComponent<BoxCollider>();

			  auto e2 = scene.CreateEntity();
			  e2.GetComponent<Transform>().position.y = -2;
			  auto& spriteR2 = e2.AddComponent<SpriteRenderer>();
			  spriteR2.sprite = Sprite(m_Tx, { 0, 0, 128, 128 }, { 128 / 2, 128 / 2});

			  auto& rb2 = e2.AddComponent<Rigidbody>();
			  auto& box2 = e2.AddComponent<BoxCollider>();
			  box2.size.x = 3;
			};
			scene1->AddSceneInitializer(initScene1);

			SceneManager::LoadScene(scene1->GetIndex());
		}

	 private:
		Texture m_Tx;
		Texture m_Tx1;
	};

}
