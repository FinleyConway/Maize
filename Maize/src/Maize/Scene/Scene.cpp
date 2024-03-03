#include "mpch.h"
#include "Maize/Scene/Scene.h"

namespace Maize {

	Scene::Scene(const std::string& sceneName, uint32_t index) : m_SceneName(sceneName), m_Index(index)
	{

	}

	entt::entity Scene::CreateEntity()
	{
		auto entity = m_Registry.create();

		m_Registry.emplace<Transform>(entity);
		m_Registry.emplace<LocalTransform>(entity);
		m_Registry.emplace<Relationship>(entity);

		return entity;
	}

	void Scene::DestroyEntity(entt::entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::Initialize()
	{
		for (const auto& system : m_Systems)
		{
			system->Initialize(m_Registry);
		}
	}

	void Scene::Update(float deltaTime)
	{
		for (const auto& system : m_Systems)
		{
			system->Update(m_Registry, deltaTime);
		}
	}

	void Scene::Shutdown()
	{
		for (const auto& system : m_Systems)
		{
			system->Shutdown(m_Registry);
		}
	}

} // Maize