#pragma once

#include "Maize/EntitySystems/System.h"
#include "Maize/Core/AnimationClip.h"
#include "Maize/Scene/Components.h"

namespace Maize {

	class AnimationSystem : public System
	{
	public:
		void OnUpdate(ECS::EntityWorld& registry, float dt) override;
	};

}