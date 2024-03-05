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
				LOG_INFO("woop");
			}
		}
	};

	struct MovableTag { };

	class CustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float deltaTime) override
		{
			if (Input::GetButtonDown(KeyCode::Space))
			{
				LOG_INFO("down");
			}

			if (Input::GetButton(KeyCode::Space))
			{
				LOG_INFO("held");
			}

			if (Input::GetButtonUp(KeyCode::Space))
			{
				LOG_INFO("up");
			}

			if (Input::GetMouseButtonDown(MouseCode::Left))
			{
				LOG_INFO("click");
			}

			if (Input::GetMouseButton(MouseCode::Left))
			{
				LOG_INFO("clicking");
			}

			if (Input::GetMouseButtonUp(MouseCode::Left))
			{
				LOG_INFO("clock");
			}

			auto pos = Input::GetMousePosition();
			//LOG_INFO("Position: x = {}, y = {}", pos.x, pos.y);
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
			entity.AddTag<MovableTag>();
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
