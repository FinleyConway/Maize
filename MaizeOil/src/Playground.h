#pragma once

#include <Maize.h>

namespace Maize {

	class CustomCustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float deltaTime) override
		{
			if (Input::GetButtonDown(KeyCode::Space))
			{
				SceneManager::LoadScene("Default");
			}
		}
	};

	struct MovableTag { };

	class CustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float deltaTime) override
		{
			auto view = registry.view<Transform, SpriteRenderer, MovableTag>();
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
			  	e1.AddTag<MovableTag>();
				auto& spriteR = e1.AddComponent<SpriteRenderer>();
				spriteR.sprite = Sprite(m_Tx, { 0, 0, 128, 128 }, { 128 / 2, 128 / 2});
			};
			scene1->AddSceneInitializer(initScene1);


			// scene 2
			auto scene2 = SceneManager::CreateScene("Default1");
			auto initScene2 = [&](Scene& scene)
			{
				scene.AddSystem<CustomCustomSystem>("CustomCustom");

				auto e1 = scene.CreateEntity();
				auto& sprite1 = e1.AddComponent<SpriteRenderer>();
				sprite1.sprite = Sprite(m_Tx1, { 0, 0, 128, 128 }, { 128 / 2, 128 / 2});
			};
			scene2->AddSceneInitializer(initScene2);

			SceneManager::LoadScene(scene1->GetIndex());
		}

	 private:
		Texture m_Tx;
		Texture m_Tx1;
	};

}
