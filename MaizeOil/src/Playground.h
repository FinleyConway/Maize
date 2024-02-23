#pragma once

#include <Maize.h>

namespace Maize {

	class Playground : public Layer
	{
	public:
		void OnAttach() override
		{
			auto camera = m_Scene.CreateEntity();
			camera.AddComponent<CameraComponent>();

			auto entity = m_Scene.CreateEntity();
			entity.AddComponent<RigidbodyComponent>().type = BodyType::Dynamic;
			entity.AddComponent<BoxColliderComponent>().restitution = 1.f;

			auto entity1 = m_Scene.CreateEntity();
			entity1.GetComponent<TransformComponent>().position.y = -1.5f;
			entity1.AddComponent<RigidbodyComponent>();
			entity1.AddComponent<BoxColliderComponent>();

			RenderingSystem::drawDebug = true;

			m_Scene.Initialize();
		}

		void OnDetach() override
		{
			m_Scene.Shutdown();
		}

		void OnUpdate(float deltaTime) override
		{
			m_Scene.Run(deltaTime);
		}

	private:
		Scene m_Scene;
	};

}
