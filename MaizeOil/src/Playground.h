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
			entity.AddComponent<RigidbodyComponent>();
			entity.AddComponent<BoxColliderComponent>();

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
