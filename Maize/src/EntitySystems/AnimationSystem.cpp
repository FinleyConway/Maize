#include "EntitySystems/AnimationSystem.h"

namespace Maize {

	void AnimationSystem::OnUpdate(ECS::EntityWorld& registry, float dt)
	{
		for (const auto& entity : registry.GetEntityGroup<AnimationComponent, SpriteComponent>())
		{
			const auto& [animation, sprite] = registry.GetComponents<AnimationComponent, SpriteComponent>(entity);

			uint32_t& frameIndex = animation.currentFrame;
			AnimationClip& animationClip = animation.states[animation.currentState];

			if (frameIndex >= animationClip.Clip().size())
			{
				frameIndex = 0;
			}

			AnimationFrame& currentFrame = animationClip.GetFrame(frameIndex);

			currentFrame.elapsedTime += dt * 1000.0f;

			if (static_cast<uint32_t>(currentFrame.elapsedTime) >= currentFrame.frameDuration)
			{
				sprite.sprite = currentFrame.sprite;

				frameIndex++;
				currentFrame.elapsedTime = 0;
			}
		}
	}

}