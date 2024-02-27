#include "mpch.h"
#include "Maize/Scene/Scene.h"
#include "Maize/Scene/Entity.h"

#include "Maize/Scene/Systems/HierarchySystem.h"

namespace Maize {

	void Scene::Initialize()
	{
		Start();
		CollisionSystem::OnStart(m_Registry);
	}

	void Scene::Run(float deltaTime)
	{
		// any systems that want to handle physics based logic
		// TODO:
		// potentially add a fix time step here
		PhysicsUpdate(deltaTime);

		// handle all game collisions
		CollisionSystem::OnUpdate(m_Registry, deltaTime);

		// any systems that want to handle collision callback logic
		PhysicsCallback(deltaTime);

		HierarchySystem::Update(m_Registry);

		// main update for systems
		Update(deltaTime);

		// any systems that want to be updated after the main update
		LateUpdate(deltaTime);

		// update all cameras
		CameraSystem::OnUpdate(m_Registry, deltaTime);

		// render all entities
		RenderingSystem::OnRender(m_Registry);

		// update entities that want to be removed
		End();
	}

	void Scene::Shutdown()
	{
		End();
		CollisionSystem::OnDestroy();
	}

	Entity Scene::CreateEntity()
	{
		Entity entity = Entity(m_Registry.create(), this);

		entity.AddComponent<Transform>();
		entity.AddComponent<LocalTransform>();
		entity.AddComponent<Relationship>();

		m_Entities.emplace_back(entity);

		return entity;
	}

	void Scene::DestroyEntity(entt::entity entity)
	{
		/*
		 * TODO:
		 * Add any error checking if a removal entity goes wrong
		 */

		auto it = std::remove(m_Entities.begin(), m_Entities.end(), entity);
		m_Entities.erase(it, m_Entities.end());

		m_Registry.destroy(entity);
	}

	void Scene::AddSystem(System*system)
	{
		m_Systems.push_back(system);
	}

	void Scene::RemoveSystem(System*system)
	{
		auto it = std::find(m_Systems.begin(), m_Systems.end(), system);
		if (it != m_Systems.end())
		{
			m_Systems.erase(it);
		}
	}

	void Scene::Start()
	{
		for (auto* system : m_Systems)
		{
			system->Start(m_Registry);
		}
	}

	void Scene::PhysicsUpdate(float deltaTime)
	{
		for (auto* system : m_Systems)
		{
			system->PhysicsUpdate(m_Registry, deltaTime);
		}
	}

	void Scene::PhysicsCallback(float deltaTime)
	{
		for (auto* system : m_Systems)
		{
			system->PhysicsCallback(m_Registry, deltaTime);
		}

		// clear physics callback
		// !!this needs to be always last in this function!!
		m_Registry.clear<TriggerEnterContact, TriggerExitContact, CollisionEnterContact, CollisionExitContact>();
	}

	void Scene::Update(float deltaTime)
	{
		for (auto* system : m_Systems)
		{
			system->Update(m_Registry, deltaTime);
		}
	}

	void Scene::LateUpdate(float deltaTime)
	{
		for (auto* system : m_Systems)
		{
			system->LateUpdate(m_Registry, deltaTime);
		}
	}

	void Scene::End()
	{
		for (auto* system : m_Systems)
		{
			system->OnEnd(m_Registry);
		}
	}

} // Maize