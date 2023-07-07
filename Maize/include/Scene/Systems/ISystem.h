#pragma once

#include <EntityWorld.h>

namespace Maize {

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		virtual void OnUpdate(ECS::EntityWorld& registry, float dt) = 0;
		virtual void OnRender(ECS::EntityWorld& registry, float dt) = 0;
	};

}