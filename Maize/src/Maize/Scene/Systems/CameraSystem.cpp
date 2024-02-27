#include "mpch.h"
#include "Maize/Scene/Systems/CameraSystem.h"

namespace Maize {

	void CameraSystem::OnUpdate(entt::registry& reg, float deltaTime)
	{
		auto view = reg.view<Transform, Camera>();
		for (auto [entity, transform, cameraComponent] : view.each())
		{
			const float ppu = 100.0f;
			const int32_t flip = -1;
			const auto& gameView = Renderer::GetCurrentTexture()->getView();
			sf::View camera;

			camera.setCenter({ transform.position.x * ppu, transform.position.y * ppu * flip });
			camera.setRotation(transform.angle);

			camera.setSize(gameView.getSize().x * cameraComponent.zoom.x * transform.scale.x, gameView.getSize().y * cameraComponent.zoom.y * transform.scale.y);

			camera.setViewport(cameraComponent.viewport);

			Renderer::GetCurrentTexture()->setView(camera);
		}
	}

} // Maize