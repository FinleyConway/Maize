#pragma once

#include "EntitySystems/System.h"
#include "Scene/Components.h"
#include "Core/AnimationClip.h"

namespace Maize {

	class AnimationSystem : public System
	{
	public:
		void OnUpdate(ECS::EntityWorld& registry, float dt) override;
	};

}