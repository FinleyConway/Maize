#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/System.h"
#include "Maize/Scene/Components.h"

namespace Maize {

	class System;

	class Scene
	{
	 public:
		explicit Scene(const std::string& sceneName, uint32_t index);

		entt::entity CreateEntity();
		void DestroyEntity(entt::entity entity);

		template<typename T, typename... Args>
		void AddSystem(Args&& ... args)
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

			m_Systems.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		};

		template<typename T>
		void RemoveSystem()
		{
			static_assert(std::is_base_of<System, T>::value, "T must be a base class of System");

			auto it = std::remove_if(m_Systems.begin(), m_Systems.end(), [](const auto& system)
			{
			  return dynamic_cast<T*>(system.get()) != nullptr;
			});

			m_Systems.erase(it, m_Systems.end());
		};

		const std::string& GetName() const
		{
			return m_SceneName;
		}

		uint32_t GetIndex() const
		{
			return m_Index;
		}

	 private:
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

} // Maize