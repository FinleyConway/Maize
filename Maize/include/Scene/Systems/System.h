#pragma once

#include <SDL.h>
#include <EntityWorld.h>

namespace Maize {

	class System
	{
	public:
		virtual ~System() = default;

		virtual void OnStart(ECS::EntityWorld& registry) { };
		virtual void OnEvent(ECS::EntityWorld& registry, const SDL_Event& event, float dt) { }
		virtual void OnUpdate(ECS::EntityWorld& registry, float dt) { };
		virtual void OnRender(ECS::EntityWorld& registry, float dt) { };
		virtual void OnEndFrame(ECS::EntityWorld& registry) { };
		virtual void OnEnd(ECS::EntityWorld& registry) { };
	};

}