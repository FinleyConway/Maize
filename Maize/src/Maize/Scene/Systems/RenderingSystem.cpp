#include "mpch.h"
#include "Maize/Scene/Systems/RenderingSystem.h"

#include "Maize/Scene/Components.h"

#include "Maize/Renderer/Renderer.h"

namespace Maize {

	void RenderingSystem::OnStart(entt::registry& registry)
	{
		auto view = registry.view<Transform, SpriteRenderer>();

		for (auto [entity, transform, spriteRenderer] : view.each())
		{
			Renderer::InsertDrawable(transform, spriteRenderer);
		}
	}

	void RenderingSystem::OnUpdate(entt::registry& registry, float update)
	{
		auto view = registry.view<Transform, SpriteRenderer>();

		for (auto [entity, transform, spriteRenderer] : view.each())
		{
			Renderer::UpdateDrawable(transform, spriteRenderer);
		}
	}

	void RenderingSystem::OnRender(entt::registry& registry)
	{
		Renderer::DrawScene();
	}

} // Maize