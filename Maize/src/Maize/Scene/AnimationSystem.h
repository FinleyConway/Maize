#pragma once

#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Scene/Components.h"

namespace Maize {

	class AnimationSystem
	{
	public:
		void OnUpdate(ECS::EntityWorld& reg, float deltaTime)
		{
			for (auto entity : reg.GetEntityGroup<TransformComponent, SpriteComponent, AnimatorComponent>())
			{
				const auto& [transform, sprite, animator] = reg.GetComponents<TransformComponent, SpriteComponent, AnimatorComponent>(entity);

				Animation& animation = animator.states[animator.currentState];
				float animationDeltaSpeed = deltaTime * animator.animationSpeed;

				animation.Update(sprite.sprite, animationDeltaSpeed);
			}
		}
	};

} // Maize
