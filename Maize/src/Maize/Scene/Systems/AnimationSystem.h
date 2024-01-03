#pragma once

#include <entt/entt.hpp>

#include "Maize/Scene/Components.h"

namespace Maize {

	class AnimationSystem
	{
	public:
		void OnUpdate(entt::registry& reg, float deltaTime)
		{
			auto view = reg.view<TransformComponent, SpriteComponent, AnimatorComponent>();
			for (auto [entity, transform, sprite, animator] : view.each())
			{
				Animation& animation = animator.states[animator.currentState];
				float animationDeltaSpeed = deltaTime * animator.animationSpeed;

				animation.Update(sprite.sprite, animationDeltaSpeed);
			}
		}
	};

} // Maize
