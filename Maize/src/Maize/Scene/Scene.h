#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/System.h"

namespace Maize {

	class Entity;

	class Scene
	{
	 public:
		explicit Scene(const std::string& sceneName, uint32_t index);

		Entity CreateEntity();
		void DestroyEntity(Entity entity);

		template<typename T, typename... Args> inline void AddSystem(const std::string& debugName, uint32_t orderPriority = 0, Args&& ... args);
		template<typename T> inline void RemoveSystem();

		const std::string& GetName() const;
		uint32_t GetIndex() const;

		void AddSceneInitializer(const std::function<void(Scene&)>& init);

	 private:
		friend class Entity;
		friend class SceneManager;

		void Initialize();
		void Update(float deltaTime);
		void Shutdown();

	 private:
		std::string m_SceneName;
		uint32_t m_Index = 0;
		bool m_IsCurrent = false;

		entt::registry m_Registry;
		std::vector<std::unique_ptr<System>> m_Systems;

		// temp since i dont know a solution to serialize scene
		std::function<void(Scene&)> m_SceneInit;
	};

	template<typename T, typename... Args>
	inline void Scene::AddSystem(const std::string& debugName, uint32_t orderPriority, Args&& ...args)
	{
		static_assert(std::is_base_of<System, T>::value, "T must be a base class of System");

		for (const auto& system : m_Systems)
		{
			if (dynamic_cast<T*>(system.get()) != nullptr)
			{
				LOG_CORE_WARN("Adding a pre-existing system");
				return;
			}
		}

		auto newSystem = std::make_unique<T>(std::forward<Args>(args)...);
		newSystem->SetOrderPriority(orderPriority);
		newSystem->SetName(debugName);

		m_Systems.emplace_back(std::move(newSystem));
	}

	template<typename T>
	inline void Scene::RemoveSystem()
	{
		static_assert(std::is_base_of<System, T>::value, "T must be a base class of System");

		auto it = std::remove_if(m_Systems.begin(), m_Systems.end(), [](const auto& system)
		{
		  return dynamic_cast<T*>(system.get()) != nullptr;
		});

		m_Systems.erase(it, m_Systems.end());
	}

} // Maize