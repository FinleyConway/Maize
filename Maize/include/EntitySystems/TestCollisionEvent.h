#pragma once

#include <box2d.h>

#include "System.h"
#include "Scene/Components.h"

namespace Maize {

	class TestCollisionEvent : public System
	{
	public:
		void OnUpdate(ECS::EntityWorld& registry, float dt) override
		{
			for (const auto entity : registry.GetEntityGroup<CollisionContactComponent, DummyComponent>())
			{
				const auto& [collisionData, dummy] = registry.GetComponents<CollisionContactComponent, DummyComponent>(entity);

				b2Body* body = (b2Body*)collisionData.body;

				if (collisionData.collideEvent == CollisionContactComponent::CollidingEvent::Enter)
				{
					body->SetTransform({ 0, 0 }, 0);

					ECS::Entity otherEntity = collisionData.entity;

					if (registry.HasComponent<AnimationComponent>(otherEntity))
					{
						auto& animationC = registry.GetComponent<AnimationComponent>(otherEntity);

						animationC.currentState = "Walking";
					}

					if (registry.HasComponent<AudioSourceComponent>(otherEntity))
					{
						auto& audio = registry.GetComponent<AudioSourceComponent>(otherEntity);

						audio.audioClips.push_back(dummy.clip);
					}
				}
				else if (collisionData.collideEvent == CollisionContactComponent::CollidingEvent::Exit)
				{

					body->SetLinearVelocity({ 0, 0 });
				}
			}
		}
	};

}