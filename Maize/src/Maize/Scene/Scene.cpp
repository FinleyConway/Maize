#include "mpch.h"
#include "Maize/Scene/Scene.h"

#include "Maize/Scene/Entity.h"
#include "Maize/Scene/Components.h"

#include "Maize/Scene/Systems/HierarchySystem.h"
#include "Maize/Scene/Systems/CollisionSystem.h"

namespace Maize {

	Scene::Scene(const std::string& sceneName, uint32_t index) : m_SceneName(sceneName), m_Index(index)
	{
	}

	Entity Scene::CreateEntity()
	{
		auto entity = Entity(m_Registry.create(), this);

		entity.AddComponent<Transform>();
		entity.AddComponent<LocalTransform>();
		entity.AddComponent<Relationship>();

		return entity;
	}

	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	const std::string& Scene::GetName() const
	{
		return m_SceneName;
	}

	uint32_t Scene::GetIndex() const
	{
		return m_Index;
	}

	void Scene::AddSceneInitializer(const std::function<void(Scene&)>& init)
	{
		m_SceneInit = init;
	}

	void Scene::Initialize()
	{
		m_SceneInit(*this);

		AddSystem<HierarchySystem>("Parent-Child", 900);
		AddSystem<CollisionSystem>("Collision", 500);

		const auto compareFunction = [&](const std::unique_ptr<System>& a, const std::unique_ptr<System>& b) {
		  return a->GetOrderPriority() < b->GetOrderPriority();
		};

		std::sort(m_Systems.begin(), m_Systems.end(), compareFunction);

		for (const auto& system : m_Systems)
		{
			MZ_LOG_CORE_INFO(system->GetName());

			system->Initialize(m_Registry);
		}
	}

	void Scene::Update(float deltaTime)
	{
		for (const auto& system : m_Systems)
		{
			if (!m_IsCurrent) return;

			system->Update(m_Registry, deltaTime);
		}
	}

	void Scene::Shutdown()
	{
		for (const auto& system : m_Systems)
		{
			system->Shutdown(m_Registry);
		}

		m_Registry.clear();
		m_Systems.clear();
	}

} // Maize