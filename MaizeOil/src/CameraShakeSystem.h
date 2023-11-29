#pragma once

#include <Maize.h>

class CameraShakeSystem
{
public:
	void Update(ECS::EntityWorld& reg, float deltaTime)
	{
		for (auto entity : reg.GetEntityGroup<Maize::TransformComponent, Maize::CameraShakeComponent>())
		{
			const auto [transform, cameraShake] = reg.GetComponents<Maize::TransformComponent, Maize::CameraShakeComponent>(entity);

			if (cameraShake.duration > 0)
			{
				cameraShake.originalPosition = transform.position;

				cameraShake.duration -= deltaTime;

				float offsetX = Maize::Random::Range(-1.0f, 1.0f);
				float offsetY = Maize::Random::Range(-1.0f, 1.0f);

				transform.position.x += offsetX * cameraShake.intensity;
				transform.position.y += offsetY * cameraShake.intensity;

				if (cameraShake.duration <= 0.0f)
				{
					transform.position = cameraShake.originalPosition;
				}
			}
		}
	}
};
