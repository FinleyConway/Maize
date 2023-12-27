#pragma once

#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Scene/Components.h"
#include "Maize/Renderer/Renderer.h"

namespace Maize {

	class CameraSystem
	{
	public:
		void OnUpdate(ECS::EntityWorld& reg, float deltaTime)
		{
			for (auto entity : reg.GetEntityGroup<TransformComponent, CameraComponent>())
			{
				const auto& [transformComponent, cameraComponent] = reg.GetComponents<TransformComponent, CameraComponent>(entity);

				const auto& view = Renderer::GetCurrentTexture()->getView();
				sf::View camera;

				camera.setCenter(transformComponent.position);
				camera.setRotation(transformComponent.angle);

				camera.setSize(view.getSize().x * cameraComponent.zoom.x * transformComponent.scale.x ,view.getSize().y * cameraComponent.zoom.y * transformComponent.scale.y);

				camera.setViewport(cameraComponent.viewport);

				Renderer::GetCurrentTexture()->setView(camera);
			}
		}
	};

} // Maize
