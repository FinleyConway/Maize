#pragma once

#include "Scene/Systems/ISystem.h"
#include "Scene/Components.h"
#include "Core/AnimationClip.h"

namespace Maize {

	class AnimationSystem : public ISystem
	{
	public:
		void OnUpdate(ECS::EntityWorld& registry, float dt) override;
		void OnRender(ECS::EntityWorld& registry, float dt) override;
	};

}