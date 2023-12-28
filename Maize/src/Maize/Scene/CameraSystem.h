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
				const auto& [transform, cameraComponent] = reg.GetComponents<TransformComponent, CameraComponent>(entity);

				const float ppu = 100.0f;
				const int32_t flip = -1;
				const auto& view = Renderer::GetCurrentTexture()->getView();
				sf::View camera;

				camera.setCenter({ transform.position.x * ppu, transform.position.y * ppu * flip });
				camera.setRotation(transform.angle);

				camera.setSize(view.getSize().x * cameraComponent.zoom.x * transform.scale.x ,view.getSize().y * cameraComponent.zoom.y * transform.scale.y);

				camera.setViewport(cameraComponent.viewport);

				Renderer::GetCurrentTexture()->setView(camera);
			}
		}
	};

} // Maize
