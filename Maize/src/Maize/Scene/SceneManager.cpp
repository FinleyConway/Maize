#include "mpch.h"
#include "Maize/Scene/SceneManager.h"

#include "Maize/Scene/Scene.h"

namespace Maize {

	void SceneManager::OnUpdate(float deltaTime)
	{
		if (s_ActiveScene != nullptr)
		{
			s_ActiveScene->Update(deltaTime);
		}
	}

	std::shared_ptr<Scene> SceneManager::CreateScene(const std::string& sceneName)
	{
		auto index = s_Scenes.size();
		auto scene = std::make_shared<Scene>(sceneName, index);

		s_Scenes.try_emplace(index, scene);

		if (s_ActiveScene == nullptr)
		{
			s_ActiveScene = scene;
		}

		return s_Scenes.at(index);
	}

	std::shared_ptr<Scene> SceneManager::GetActiveScene()
	{
		return s_Scenes.at(s_LoadedScene);
	}

	std::shared_ptr<Scene> SceneManager::GetSceneAt(uint32_t sceneIndex)
	{
		if (s_Scenes.contains(sceneIndex))
		{
			return s_Scenes.at(sceneIndex);
		}

		return nullptr;
	}

	std::shared_ptr<Scene> SceneManager::GetSceneAt(const std::string& sceneName)
	{
		for (const auto& [index, scene] : s_Scenes)
		{
			if (scene->GetName() == sceneName)
			{
				return scene;
			}
		}

		return nullptr;
	}

	void SceneManager::LoadScene(uint32_t sceneIndex)
	{
		Load(GetSceneAt(sceneIndex));
	}

	void SceneManager::LoadScene(const std::string& sceneName)
	{
		Load(GetSceneAt(sceneName));
	}

	void SceneManager::Load(const std::shared_ptr<Scene>& scene)
	{
		if (scene == nullptr)
		{
			std::cerr << "Trying to load a non-existent scene!" << std::endl;
			return;
		}

		if (s_ActiveScene != nullptr)
		{
			s_ActiveScene->Shutdown();
			s_ActiveScene = nullptr;
		}

		s_ActiveScene = scene;
		s_ActiveScene->Initialize();
		s_LoadedScene = s_ActiveScene->GetIndex();
	}

} // Maize