#include "mpch.h"
#include "Maize/Scene/Systems/RenderingSystem.h"

namespace Maize {

	void RenderingSystem::OnRender(ECS::EntityWorld& registry)
	{
		RenderSprites(registry);
	}

	void RenderingSystem::RenderSprites(ECS::EntityWorld& registry)
	{
		/*
		 * TODO: add camera
		*/

		SpriteBatches batch;
		GetDrawables(registry, batch);
		SortDrawables(batch);
		RenderSprite(batch);
	}

	void RenderingSystem::GetDrawables(ECS::EntityWorld& registry, SpriteBatches& batches) const
	{
		batches.clear();

		// sprite rendering
		for (auto entity : registry.GetEntityGroup<TransformComponent, SpriteComponent>())
		{
			const auto& [transform, sprite] = registry.GetComponents<TransformComponent, SpriteComponent>(entity);

			if (sprite.sprite.IsEmpty()) continue;

			/*
			* TODO: add culling so it doesnt render every sprite in the scene
			*/

			sf::Sprite& spriteInfo = sprite.sprite.GetSprite();

			spriteInfo.setPosition(transform.position);
			spriteInfo.setRotation(transform.angle);
			spriteInfo.setScale(transform.scale);

			batches[sprite.sortingLayer].emplace_back(sprite.orderInLayer, &spriteInfo);
		}
	}

	void RenderingSystem::SortDrawables(SpriteBatches& batches)
	{
		auto SortLayer = [](const std::pair<int32_t, const sf::Sprite*>& sprite1, const std::pair<int32_t, const sf::Sprite*>& sprite2)
		{
			// compare textures
			if (sprite1.second->getTexture() != sprite2.second->getTexture())
			{
				return sprite1.second->getTexture() < sprite2.second->getTexture();
			}

			// compare layer order if textures are the same
			return sprite1.first < sprite2.first;
		};

		for (auto& [_, batch] : batches)
		{
			std::sort(batch.begin(), batch.end(), SortLayer);
		}
	}

	void RenderingSystem::RenderSprite(SpriteBatches& batches)
	{
		m_Renderer.BeginSceneDrawing();

		for (auto& [layer, batch] : batches)
		{
			for (auto& [_, sprite] : batch)
			{
				m_Renderer.Draw(*sprite, m_Renderer.GetCurrentTexture());
			}
		}

		m_Renderer.EndSceneDrawing();
	}

} // Maize