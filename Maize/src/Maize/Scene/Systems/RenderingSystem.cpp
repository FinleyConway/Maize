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
			auto& sprite = spriteRenderer.sprite;

			const float ppu = 100.0f;
			const int8_t flip = -1;

			const float scaling = ppu / sprite.GetPixelPerUnit();

			sprite.setPosition(transform.position.x * ppu, transform.position.y * ppu * flip);
			sprite.setRotation(transform.angle);
			sprite.setScale(transform.scale.x * scaling, transform.scale.y * scaling * flip);

			Renderer::InsertDrawable({ &sprite, sprite.GetGlobalBounds(), spriteRenderer.sortingLayer, spriteRenderer.orderInLayer });
		}
	}

	void RenderingSystem::OnUpdate(entt::registry& registry, float update)
	{
		auto view = registry.view<Transform, SpriteRenderer>();

		for (auto [entity, transform, spriteRenderer] : view.each())
		{
			auto& sprite = spriteRenderer.sprite;

			const float ppu = 100.0f;
			const int8_t flip = -1;

			const float scaling = ppu / sprite.GetPixelPerUnit();

			sprite.setPosition(transform.position.x * ppu, transform.position.y * ppu * flip);
			sprite.setRotation(transform.angle);
			sprite.setScale(transform.scale.x * scaling, transform.scale.y * scaling * flip);

			Renderer::UpdateDrawable({ &sprite, sprite.GetGlobalBounds(), spriteRenderer.sortingLayer, spriteRenderer.orderInLayer });
		}
	}

	void RenderingSystem::OnRender(entt::registry& registry)
	{
		Renderer::DrawScene();
	}

} // Maize