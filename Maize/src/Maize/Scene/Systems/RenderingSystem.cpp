#include "mpch.h"
#include "Maize/Scene/Systems/RenderingSystem.h"

#include "Maize/Scene/Components.h"

#include "Maize/Renderer/Renderer.h"

namespace Maize {

	void RenderingSystem::Initialize(entt::registry& registry)
	{
		auto view = registry.view<Transform, SpriteRenderer>();

		for (auto [entity, transform, spriteRenderer] : view.each())
		{
			Renderer::InsertDrawable(transform, spriteRenderer);
		}
	}

	void RenderingSystem::Update(entt::registry& registry, float update)
	{
		auto view = registry.view<Transform, SpriteRenderer>();

		for (auto [entity, transform, spriteRenderer] : view.each())
		{
			Renderer::UpdateDrawable(transform, spriteRenderer);
		}

		Renderer::DrawScene();
	}

	void RenderingSystem::Shutdown(entt::registry& registry)
	{
		auto view = registry.view<Transform, SpriteRenderer>();

		for (auto [entity, transform, spriteRenderer] : view.each())
		{
			Renderer::RemoveDrawable(&spriteRenderer.sprite);
		}
	}

} // Maize