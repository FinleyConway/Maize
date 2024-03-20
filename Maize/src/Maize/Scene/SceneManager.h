#pragma once

#include "Maize/Core/Layer.h"

namespace Maize {

	class Scene;

	class SceneManager
	{
	 public:
		void OnFixedUpdate(float fixedDeltaTime);
		void OnUpdate(float deltaTime);

		static std::shared_ptr<Scene> CreateScene(const std::string& sceneName);

		static std::shared_ptr<Scene> GetActiveScene();
		static std::shared_ptr<Scene> GetSceneAt(uint32_t sceneIndex);
		static std::shared_ptr<Scene> GetSceneAt(const std::string& sceneName);

		static void LoadScene(uint32_t sceneIndex);
		static void LoadScene(const std::string& sceneName);

	 private:
		static void Load(const std::shared_ptr<Scene>& scene);

		inline static std::map<uint32_t, std::shared_ptr<Scene>> s_Scenes;
		inline static uint32_t s_LoadedScene = 0;

		inline static std::shared_ptr<Scene> s_ActiveScene = nullptr;
	};

} // Maize