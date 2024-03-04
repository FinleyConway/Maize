#pragma once

#include <Maize.h>

namespace Maize {

	bool hasPressed = false;

	class CustomCustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float update) override
		{
			if (Input::IsKeyPressed(KeyCode::Space))
			{
				if (!hasPressed)
				{
					SceneManager::LoadScene("Default");
					hasPressed = true;
				}
			}
			else
			{
				hasPressed = false;
			}
		}
	};

	class CustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float update) override
		{
			if (Input::IsKeyPressed(KeyCode::Space))
			{
				if (!hasPressed)
				{
					SceneManager::LoadScene("Default1");
					hasPressed = true;
				}
			}
			else
			{
				hasPressed = false;
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
			auto sprite = Sprite(m_Tx, { 0, 0, 128, 128 }, { 128 / 2, 128 / 2});
			auto sprite1 = Sprite(m_Tx1, { 0, 0, 128, 128 }, { 128 / 2, 128 / 2});

			auto scene = SceneManager::CreateScene("Default");

			scene->AddSystem<CustomSystem>("Custom");

			auto entity = scene->CreateEntity();
			auto& spriteR = entity.AddComponent<SpriteRenderer>();
			spriteR.sprite = sprite;

			auto scene1 = SceneManager::CreateScene("Default1");

			scene1->AddSystem<CustomCustomSystem>("CustomCustom");

			auto entity1 = scene1->CreateEntity();
			auto& spriteR1 = entity1.AddComponent<SpriteRenderer>();
			spriteR1.sprite = sprite1;

			SceneManager::LoadScene(scene->GetIndex());
		}

	 private:
		Texture m_Tx;
		Texture m_Tx1;
	};

}
