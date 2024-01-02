#include "mpch.h"
#include "RenderingSystem.h"
#include "Maize/Math/Math.h"

namespace Maize {

	void RenderingSystem::OnRender(ECS::EntityWorld& reg)
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
			RenderDebugColliders(reg, camera);
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

	void RenderingSystem::GetSprites(ECS::EntityWorld& reg, const sf::View& camera, std::unordered_map<std::string, std::vector<Drawable>>& drawableOrder)
	{
		const int32_t flip = -1; // make sure so that up is positive y and down is negative y when drawing
		const float ppu = 100.0f; // scaling factor

		for (auto entity : reg.GetEntityGroup<TransformComponent, SpriteComponent>())
		{
			const auto& [transform, spriteComponent] = reg.GetComponents<TransformComponent, SpriteComponent>(entity);

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

	void RenderingSystem::RenderDebugColliders(ECS::EntityWorld& reg, const sf::View& camera)
	{
		if (!drawDebug) return;

		const int32_t flip = -1; // make sure up is positive y and down is negative y when drawing
		const float ppu = 100.0f;

		sf::RectangleShape colliderRectShape;
		colliderRectShape.setFillColor(sf::Color::Transparent);
		colliderRectShape.setOutlineColor(sf::Color::Green);
		colliderRectShape.setOutlineThickness(1.0f);

		sf::CircleShape colliderCircleShape1;
		colliderCircleShape1.setFillColor(sf::Color::Transparent);
		colliderCircleShape1.setOutlineColor(sf::Color::Green);
		colliderCircleShape1.setOutlineThickness(1.0f);

		sf::CircleShape colliderCircleShape2;
		colliderCircleShape2.setFillColor(sf::Color::Transparent);
		colliderCircleShape2.setOutlineColor(sf::Color::Green);
		colliderCircleShape2.setOutlineThickness(1.0f);

		for (auto entity : reg.GetEntityGroup<TransformComponent, BoxColliderComponent>())
		{
			const auto& [transform, boxCollider] = reg.GetComponents<TransformComponent, BoxColliderComponent>(entity);

			const sf::Vector2f colliderSize = sf::Vector2f(boxCollider.size.x * ppu, boxCollider.size.y * ppu);
			const sf::Vector2f colliderPosition = sf::Vector2f(transform.position.x * ppu, transform.position.y * flip * ppu);
			const sf::Vector2f rotatedOffset = Rotate(sf::Vector2f(boxCollider.offset.x * ppu, boxCollider.offset.y * ppu * flip), -transform.angle);

			colliderRectShape.setOrigin(colliderSize / 2.0f);

			colliderRectShape.setSize(colliderSize);
			colliderRectShape.setScale(transform.scale.x, transform.scale.y);

			colliderRectShape.setRotation(-transform.angle);
			colliderRectShape.setPosition(colliderPosition + rotatedOffset);

			if (GetViewSpace(camera).intersects(colliderRectShape.getGlobalBounds()))
			{
				Renderer::Draw(colliderRectShape);
			}
		}

		for (auto entity : reg.GetEntityGroup<TransformComponent, CircleColliderComponent>())
		{
			const auto& [transform, circleCollider] = reg.GetComponents<TransformComponent, CircleColliderComponent>(entity);

			const float scaledRadius = (circleCollider.radius * ppu) * Math::Max(transform.scale.x, transform.scale.y);
			const sf::Vector2f colliderPosition = sf::Vector2f(transform.position.x * ppu, transform.position.y * flip * ppu);
			const sf::Vector2f rotatedOffset = Rotate(sf::Vector2f(circleCollider.offset.x * ppu, circleCollider.offset.y * ppu * flip), -transform.angle);

			colliderCircleShape1.setOrigin(scaledRadius, scaledRadius);

			colliderCircleShape1.setRadius(scaledRadius);

			colliderCircleShape1.setRotation(-transform.angle);
			colliderCircleShape1.setPosition(colliderPosition + rotatedOffset);

			if (GetViewSpace(camera).intersects(colliderCircleShape1.getGlobalBounds()))
			{
				Renderer::Draw(colliderCircleShape1);
			}
		}

		for (auto entity : reg.GetEntityGroup<TransformComponent, CapsuleColliderComponent>())
		{
			const auto& [transform, capsuleCollider] = reg.GetComponents<TransformComponent, CapsuleColliderComponent>(entity);

			const sf::Vector2f scale = sf::Vector2f(Math::Abs(transform.scale.x), Math::Abs(transform.scale.y));
			const float signX = Math::Sign(transform.scale.x);
			const float signY = Math::Sign(transform.scale.y);

			const sf::Vector2f colliderSize = sf::Vector2f(capsuleCollider.size.x * ppu, capsuleCollider.size.y * ppu);

			const float scaledCircleRadius = (capsuleCollider.size.x * ppu / 2.0f) * scale.x;
			const sf::Vector2f colliderPosition = sf::Vector2f(transform.position.x * ppu, transform.position.y * flip * ppu);
			const sf::Vector2f rotatedOffset = Rotate(sf::Vector2f(capsuleCollider.offset.x * signX * ppu, capsuleCollider.offset.y * signY * ppu * flip), -transform.angle);

			colliderRectShape.setSize({ colliderSize.x * scale.x, (colliderSize.x - capsuleCollider.size.y) * scale.y });
			colliderRectShape.setOrigin(colliderRectShape.getSize() / 2.0f);
			colliderRectShape.setPosition(colliderPosition.x + rotatedOffset.x, colliderPosition.y + rotatedOffset.y);
			colliderRectShape.setRotation(-transform.angle);

			const sf::Vector2f circleOffset1 = Rotate(sf::Vector2f(capsuleCollider.offset.x * signX * ppu, (-capsuleCollider.size.y * scale.y / 4.0f - capsuleCollider.offset.y * signY) * ppu), -transform.angle);
			const sf::Vector2f circleOffset2 = Rotate(sf::Vector2f(capsuleCollider.offset.x * signX * ppu, (capsuleCollider.size.y * scale.y / 4.0f - capsuleCollider.offset.y * signY) * ppu), -transform.angle);

			colliderCircleShape1.setRadius(scaledCircleRadius);
			colliderCircleShape1.setOrigin(colliderCircleShape1.getRadius(), colliderCircleShape1.getRadius());
			colliderCircleShape1.setPosition(colliderPosition.x + circleOffset1.x, colliderPosition.y + circleOffset1.y);

			colliderCircleShape2.setRadius(scaledCircleRadius);
			colliderCircleShape2.setOrigin(colliderCircleShape2.getRadius(), colliderCircleShape2.getRadius());
			colliderCircleShape2.setPosition(colliderPosition.x + circleOffset2.x, colliderPosition.y + circleOffset2.y);

			if (GetViewSpace(camera).intersects(colliderRectShape.getGlobalBounds()))
			{
				Renderer::Draw(colliderRectShape);
				Renderer::Draw(colliderCircleShape1);
				Renderer::Draw(colliderCircleShape2);
			}
		}

	}

} // Maize