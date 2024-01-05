#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/Components.h"
#include "Maize/Scene/System.h"

#include "Maize/Scene/Systems/CollisionSystem.h"
#include "Maize/Scene/Systems/CameraSystem.h"
#include "Maize/Scene/Systems/RenderingSystem.h"

namespace Maize {

	class Entity;

	class Scene
	{
	public:
		Scene() = default;

		void Initialize();
		void Run(float deltaTime);
		void Shutdown();

		Entity CreateEntity();
		void DestroyEntity(entt::entity entity);

		/*
		 * TODO:
		 * Decide if this will be a runtime feature.
		 */
		void AddSystem(System* system);
		void RemoveSystem(System* system);

	private:
		void Start();

		void PhysicsUpdate(float deltaTime);
		void PhysicsCallback(float deltaTime);

		void Update(float deltaTime);
		void LateUpdate(float deltaTime);

		void End();

	private:
		friend class Entity;

		entt::registry m_Registry;
		std::vector<entt::entity> m_Entities;
		std::vector<System*> m_Systems;
	};

} // Maize