#include "mpch.h"
#include "RenderingSystem.h"
#include "Maize/Math/Math.h"

namespace Maize {

	void RenderingSystem::OnRender(entt::registry& reg)
	{
		Renderer::BeginSceneDrawing();

		auto view = reg.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			std::unordered_map<std::string, std::vector<Drawable>> drawableOrder;
			const auto& gameView = Renderer::GetCurrentTexture()->getView();

			// gather drawables
			GetSprites(reg, gameView, drawableOrder);

			// draw
			RenderDrawables(drawableOrder);
			RenderDebugColliders(reg, gameView);
		}

		Renderer::EndSceneDrawing();
	}

	sf::FloatRect RenderingSystem::GetViewSpace(const sf::View& camera)
	{
		const sf::Vector2f viewSize = camera.getSize();
		const sf::Vector2f viewSizeHalf = sf::Vector2f(viewSize.x / 2.0f, viewSize.y / 2.0f);

		sf::Transform transform;
		transform.translate(camera.getCenter());
		transform.rotate(camera.getRotation());
		transform.translate(-viewSizeHalf);

		return transform.transformRect(sf::FloatRect(0.0f, 0.0f, viewSize.x, viewSize.y));
	}

	void RenderingSystem::GetSprites(entt::registry& reg, const sf::View& camera, std::unordered_map<std::string, std::vector<Drawable>>& drawableOrder)
	{
		const int32_t flip = -1; // make sure so that up is positive y and down is negative y when drawing
		const float ppu = 100.0f; // scaling factor

		auto view = reg.view<TransformComponent, SpriteComponent>();
		for (auto [entity, transform, spriteComponent] : view.each())
		{
			const sf::Vector2f spritePosition = sf::Vector2f(transform.position.x, transform.position.y * flip) * ppu;

			spriteComponent.sprite.setPosition(spritePosition);

			if (GetViewSpace(camera).intersects(spriteComponent.sprite.GetGlobalBounds()))
			{
				if (spriteComponent.sprite.GetTexture() != nullptr)
				{
					spriteComponent.sprite.setRotation(-transform.angle);

					float scaling = ppu / spriteComponent.sprite.GetPixelPerUnit();
					spriteComponent.sprite.setScale(transform.scale.x * scaling, transform.scale.y * scaling);

					if (spriteComponent.flipX)
					{
						spriteComponent.sprite.setScale(-spriteComponent.sprite.getScale().x, spriteComponent.sprite.getScale().y);
					}

					if (spriteComponent.flipY)
					{
						spriteComponent.sprite.setScale(spriteComponent.sprite.getScale().x, -spriteComponent.sprite.getScale().y);
					}

					drawableOrder[spriteComponent.sortingLayer].emplace_back(Drawable(static_cast<const sf::Drawable*>(&spriteComponent.sprite), spriteComponent.orderInLayer));
				}
			}
		}
	}

	void RenderingSystem::RenderDrawables(std::unordered_map<std::string, std::vector<Drawable>>& drawableOrder)
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

	void RenderingSystem::RenderDebugColliders(entt::registry& reg, const sf::View& camera)
	{
		if (!drawDebug) { return; }

		const int32_t flip = -1; // make sure up is positive y and down is negative y when drawing
		const float ppu = 100.0f;

		sf::RectangleShape colliderRectShape;
		colliderRectShape.setFillColor(sf::Color::Transparent);
		colliderRectShape.setOutlineColor(sf::Color::Green);
		colliderRectShape.setOutlineThickness(1.0f);

		sf::CircleShape colliderCircleShape;
		colliderCircleShape.setFillColor(sf::Color::Transparent);
		colliderCircleShape.setOutlineColor({ 0, 255, 0, 127 });
		colliderCircleShape.setOutlineThickness(1.0f);

		auto view = reg.view<TransformComponent, RigidbodyComponent>();
		for (auto [entity, transform, rigidbody] : view.each())
		{
			// add a collider if that entity has this component
			if (reg.all_of<BoxColliderComponent>(entity))
			{
				auto& boxCollider = reg.get<BoxColliderComponent>(entity);

				const sf::Vector2f colliderSize = sf::Vector2f(boxCollider.size.x * ppu, boxCollider.size.y * ppu);
				const sf::Vector2f colliderPosition = sf::Vector2f(transform.position.x * ppu, transform.position.y * flip * ppu);
				const sf::Vector2f rotatedOffset = Rotate(sf::Vector2f(boxCollider.offset.x * ppu, boxCollider.offset.y * ppu * flip), -transform.angle);

				colliderRectShape.setOrigin(colliderSize / 2.0f);

				colliderRectShape.setSize(colliderSize);
				colliderRectShape.setScale(transform.scale.x, transform.scale.y);

				colliderRectShape.setRotation(-transform.angle);
				colliderRectShape.setPosition(colliderPosition + rotatedOffset);

				if (rigidbody.body->IsAwake())
				{
					colliderRectShape.setOutlineColor({ 0, 255, 0, 255 });
				}
				else
				{
					colliderRectShape.setOutlineColor({ 0, 255, 0, 127 });
				}

				if (GetViewSpace(camera).intersects(colliderRectShape.getGlobalBounds()))
				{
					Renderer::Draw(colliderRectShape);
				}
			}

			// add a collider if that entity has this component
			if (reg.all_of<CircleColliderComponent>(entity))
			{
				auto& circleCollider = reg.get<CircleColliderComponent>(entity);

				const float scaledRadius = (circleCollider.radius * ppu) * Math::Max(transform.scale.x, transform.scale.y);
				const sf::Vector2f colliderPosition = sf::Vector2f(transform.position.x * ppu, transform.position.y * flip * ppu);
				const sf::Vector2f rotatedOffset = Rotate(sf::Vector2f(circleCollider.offset.x * ppu, circleCollider.offset.y * ppu * flip), -transform.angle);

				colliderCircleShape.setOrigin(scaledRadius, scaledRadius);

				colliderCircleShape.setRadius(scaledRadius);

				colliderCircleShape.setRotation(-transform.angle);
				colliderCircleShape.setPosition(colliderPosition + rotatedOffset);

				if (rigidbody.body->IsAwake())
				{
					colliderCircleShape.setOutlineColor({ 0, 255, 0, 255 });
				}
				else
				{
					colliderCircleShape.setOutlineColor({ 0, 255, 0, 127 });
				}

				if (GetViewSpace(camera).intersects(colliderCircleShape.getGlobalBounds()))
				{
					Renderer::Draw(colliderCircleShape);
				}
			}
		}
	}

} // Maize