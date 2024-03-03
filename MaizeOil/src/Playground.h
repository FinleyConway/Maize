#pragma once

#include <Maize.h>

namespace Maize {

	class CustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float update) override
		{
			std::cout << "hello!" << std::endl;
		}
	};

	class CustomCustomSystem : public System
	{
	 public:
		void Update(entt::registry& registry, float update) override
		{
			std::cout << "good bye!" << std::endl;
		}
	};

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			auto scene = SceneManager::CreateScene("Default");
			scene->AddSystem<CustomCustomSystem>(0);
			scene->AddSystem<CustomSystem>(1);

		}

	};

}
