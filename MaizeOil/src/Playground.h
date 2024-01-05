#pragma once

#include <entt/entt.hpp>
#include <Maize.h>

#include "Maize/Scene/Systems/RenderingSystem.h"
#include "Maize/Scene/Systems/CameraSystem.h"
#include "Maize/Scene/Systems/CollisionSystem.h"

namespace Maize {

	struct PlayerTag { };

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			m_Camera = m_Registry.create();

			m_Registry.emplace<TransformComponent>(m_Camera);
			m_Registry.emplace<CameraComponent>(m_Camera);

			m_Player = m_Registry.create();

			auto& t = m_Registry.emplace<TransformComponent>(m_Player);
			t.position.y = 2;
			t.angle = 90;
			auto& r = m_Registry.emplace<RigidbodyComponent>(m_Player);
			r.type = BodyType::Dynamic;
			m_Registry.emplace<CircleColliderComponent>(m_Player);
			m_Registry.emplace<PlayerTag>(m_Player);

			m_Platform = m_Registry.create();

			m_Registry.emplace<TransformComponent>(m_Platform);
			m_Registry.emplace<RigidbodyComponent>(m_Platform);
			m_Registry.emplace<BoxColliderComponent>(m_Platform);

			CollisionSystem::OnStart(m_Registry);
			RenderingSystem::drawDebug = true;
		}

		void OnDetach() override
		{
			CollisionSystem::OnDestroy();
		}

		void OnUpdate(float deltaTime) override
		{
			CollisionSystem::OnUpdate(m_Registry, deltaTime);

			// physic callback logic
			// here
			auto view = m_Registry.view<CollisionEnterContactComponent>();
			for (auto entity : view)
			{
				if(m_Registry.all_of<PlayerTag>(entity))
				{
					auto& r = m_Registry.get<RigidbodyComponent>(entity);
					auto& t = m_Registry.get<TransformComponent>(entity);
					r.body->ApplyForceToCenter({ 0, 600 }, true);
					r.isContinuous = true;
				}
			}

			// remove callback components
			auto cEnter = m_Registry.view<CollisionEnterContactComponent>();
			auto cExit = m_Registry.view<CollisionExitContactComponent>();

			for (auto entity : cEnter) m_Registry.remove<CollisionEnterContactComponent>(entity);
			for (auto entity : cExit) m_Registry.remove<CollisionExitContactComponent>(entity);

			// rest of the system
			m_CameraSystem.OnUpdate(m_Registry, deltaTime);
			RenderingSystem::OnRender(m_Registry);
		}

	private:
		CameraSystem m_CameraSystem;

		entt::registry m_Registry;

		entt::entity m_Camera;
		entt::entity m_Player;
		entt::entity m_Platform;
	};

}
