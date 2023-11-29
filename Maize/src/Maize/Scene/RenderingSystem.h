#pragma once

#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Scene/Components.h"
#include "Maize/Renderer/Renderer.h"

namespace Maize {

	class RenderingSystem
	{
	public:
		void OnRender(ECS::EntityWorld& reg)
		{
			Renderer::BeginSceneDrawing();

			for (auto entity : reg.GetEntityGroup<TransformComponent, CameraComponent>())
			{
				std::unordered_map<std::string, std::vector<Drawable>> drawableOrder;
				const auto& camera = Renderer::GetCurrentTexture()->getView();

				// gather drawables
				GetSprites(reg, camera, drawableOrder);

				// draw
				RenderDrawables(drawableOrder);
			}

			Renderer::EndSceneDrawing();
		}

	private:
		struct Drawable
		{
			const sf::Drawable* drawable = nullptr;
			int32_t orderInLayer = 0;
		};

		sf::FloatRect GetViewSpace(const sf::View& camera) const
		{
			sf::Vector2f viewSize = camera.getSize();
			sf::Vector2f viewSizeHalf = sf::Vector2f(viewSize.x / 2.0f, viewSize.y / 2.0f);

			sf::Transform transform;
			transform.translate(camera.getCenter());
			transform.rotate(camera.getRotation());
			transform.translate(-viewSizeHalf);

			return transform.transformRect(sf::FloatRect(0.0f, 0.0f, viewSize.x, viewSize.y));
		}

		void GetSprites(ECS::EntityWorld& reg, const sf::View& camera, std::unordered_map<std::string, std::vector<Drawable>>& drawableOrder)
		{
			for (auto entity : reg.GetEntityGroup<TransformComponent, SpriteComponent>())
			{
				auto [transformComponent, spriteComponent] = reg.GetComponents<TransformComponent, SpriteComponent>(entity);

				sf::FloatRect spriteBounds = spriteComponent.sprite.GetGlobalBounds();

				// check if sprite bounds is within camera bounds
				if (GetViewSpace(camera).intersects(spriteBounds))
				{
					// Set sprite properties based on transform component
					spriteComponent.sprite.setPosition(transformComponent.position);
					spriteComponent.sprite.setRotation(transformComponent.angle);
					spriteComponent.sprite.setScale(transformComponent.scale);

					if (spriteComponent.sprite.GetTexture() != nullptr)
					{
						drawableOrder[spriteComponent.sortingLayer].emplace_back(Drawable(static_cast<const sf::Drawable*>(&spriteComponent.sprite), spriteComponent.orderInLayer));
					}
				}
			}
		}

		void RenderDrawables(std::unordered_map<std::string, std::vector<Drawable>>& drawableOrder)
		{
			// loop through each layer
			for (auto& [layer, order] : drawableOrder)
			{
				// sort the sprites in that layer
				std::sort(order.begin(), order.end(), [](const Drawable& drawable1, const Drawable& drawable2)
				{
					return drawable1.orderInLayer < drawable2.orderInLayer;
				});

				for (const auto& drawable : order)
				{
					Renderer::Draw(*drawable.drawable);
				}
			}
		}
	};

} // Maize
