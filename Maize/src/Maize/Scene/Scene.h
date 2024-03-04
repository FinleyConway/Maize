#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/System.h"
#include "Maize/Scene/Components.h"

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

	 private:
		friend class Entity;
		friend class SceneManager;

		void Initialize();
		void Update(float deltaTime);
		void Shutdown();

	 private:
		std::string m_SceneName;
		uint32_t m_Index = 0;

		entt::registry m_Registry;
		std::vector<std::unique_ptr<System>> m_Systems;
	};

	template<typename T, typename... Args>
	inline void Scene::AddSystem(const std::string& debugName, uint32_t orderPriority, Args&& ...args)
	{
		static_assert(std::is_base_of<System, T>::value, "T must be a base class of System");

		for (const auto& system : m_Systems)
		{
			if (dynamic_cast<T*>(system.get()) != nullptr)
			{
				std::cerr << "Adding a pre-existing system" << std::endl;
				return;
			}
		}

		auto newSystem = std::make_unique<T>(std::forward<Args>(args)...);
		newSystem->SetOrderPriority(orderPriority);
		newSystem->SetName(debugName);

		m_Systems.emplace_back(std::move(newSystem));
	};

	template<typename T>
	inline void Scene::RemoveSystem()
	{
		static_assert(std::is_base_of<System, T>::value, "T must be a base class of System");

		auto it = std::remove_if(m_Systems.begin(), m_Systems.end(), [](const auto& system)
		{
		  return dynamic_cast<T*>(system.get()) != nullptr;
		});

		m_Systems.erase(it, m_Systems.end());
	};

} // Maize