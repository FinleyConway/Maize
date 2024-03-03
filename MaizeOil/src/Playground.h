#pragma once

#include <Maize.h>

namespace Maize {

	bool hasPressed = false;

	class CustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float update) override
		{
			std::cout << "hello!" << std::endl;

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

	class CustomCustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float update) override
		{
			std::cout << "good bye!" << std::endl;

			if (Input::IsKeyPressed(KeyCode::Space))
			{
				if (!hasPressed)
				{
					SceneManager::LoadScene(SceneManager::GetActiveScene()->GetIndex() - 1);
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
			auto scene = SceneManager::CreateScene("Default");
			scene->AddSystem<CustomSystem>();

			auto scene1 = SceneManager::CreateScene("Default1");
			scene1->AddSystem<CustomCustomSystem>();
		}

	};

}
