#include "Scene/Systems/RenderingSystem.h"

namespace Maize {

	RenderingSystem::RenderingSystem(Renderer& renderer) :
		m_Renderer(renderer)
	{
	}

	void RenderingSystem::OnUpdate(ECS::EntityWorld& registry, float dt) { return; }

    void RenderingSystem::OnRender(ECS::EntityWorld& registry, float dt)
    {
        m_Renderer.Clear();

        // iterate through the camera entities
        for (const auto& cameraEntity : registry.GetEntityGroup<TransformComponent, CameraComponent>())
        {
            const auto& [cameraTransform, camera] = registry.GetComponents<TransformComponent, CameraComponent>(cameraEntity);

            RenderSprites(registry, CameraData(cameraTransform, camera));
        }

        m_Renderer.Present();
    }

    void RenderingSystem::RenderSprites(ECS::EntityWorld& registry, const CameraData& cameraData)
    {
        std::unordered_map<Texture*, std::vector<SpriteRenderData>> spriteBatches;
        GetSpriteRenderData(registry, cameraData, spriteBatches);
        SortSpriteBatches(spriteBatches);

        // render each batch
        for (const auto& [_, renderDataList] : spriteBatches)
        {
            for (const auto& renderData : renderDataList)
            {
                RenderSprite(renderData, cameraData);
            }
        }
    }

    void RenderingSystem::GetSpriteRenderData(ECS::EntityWorld& registry, const CameraData& cameraData, std::unordered_map<Texture*, std::vector<SpriteRenderData>>& spriteBatches)
    {
        spriteBatches.clear();

        for (const auto& entity : registry.GetEntityGroup<TransformComponent, SpriteComponent>())
        {
            const auto& [transform, sprite] = registry.GetComponents<TransformComponent, SpriteComponent>(entity);

            if (sprite.sprite != nullptr)
            {
                SpriteRenderData renderData(&transform, &sprite);

                if (IsVisibleInCamera(renderData, cameraData))
                {
                    spriteBatches[&sprite.sprite->Tex()].emplace_back(renderData);
                }
            }
        }
    }

    /*
    * Needs refactoring!!
    */
    bool RenderingSystem::IsVisibleInCamera(const SpriteRenderData& renderData, const CameraData& cameraData) const
    {
        const auto RotateVector = [](PointF vector, float angle)
        {
            const float cosAngle = cos(angle);
            const float sinAngle = sin(angle);

            const float x = vector.x * cosAngle - vector.y * sinAngle;
            const float y = vector.x * sinAngle + vector.y * cosAngle;

            return PointF(x, y);
        };

        const PointF& spritePosition = renderData.transform->position;
        const Point& spriteSize = Point(renderData.sprite->sprite->Position().w, renderData.sprite->sprite->Position().h);
        const float spriteRotation = renderData.transform->angle;

        const PointF& cameraPosition = cameraData.transform.position;
        const Point& cameraSize = cameraData.camera.bounds;

        // Transform sprite position based on rotation
        const PointF rotatedSpritePosition = RotateVector(spritePosition, spriteRotation);

        // Transform sprite size based on rotation
        const Point rotatedSpriteSize(
            abs(static_cast<int>(spriteSize.x * cos(spriteRotation))) + abs(static_cast<int>(spriteSize.y * sin(spriteRotation))),
            abs(static_cast<int>(spriteSize.x * sin(spriteRotation))) + abs(static_cast<int>(spriteSize.y * cos(spriteRotation)))
        );

        const float spriteRight = rotatedSpritePosition.x + static_cast<float>(rotatedSpriteSize.x);
        const float spriteBottom = rotatedSpritePosition.y + static_cast<float>(rotatedSpriteSize.y);
        const float cameraRight = cameraPosition.x + static_cast<float>(cameraSize.x);
        const float cameraBottom = cameraPosition.y + static_cast<float>(cameraSize.y);

        return spriteRight > cameraPosition.x &&
            rotatedSpritePosition.x < cameraRight &&
            spriteBottom > cameraPosition.y &&
            rotatedSpritePosition.y < cameraBottom;
    }

    void RenderingSystem::SortSpriteBatches(std::unordered_map<Texture*, std::vector<SpriteRenderData>>& spriteBatches) const
    {
        // sort batch based on entities y position
        for (auto& [_, renderDataList] : spriteBatches)
        {
            std::ranges::sort(renderDataList.begin(), renderDataList.end(), [](const auto& a, const auto& b) {
                return a.transform->position.y < b.transform->position.y;
            });
        }
    }

    SDL_RendererFlip RenderingSystem::FlipSprite(const SpriteComponent& sprite) const
    {
        SDL_RendererFlip flip = SDL_FLIP_NONE;

        if (sprite.flipX)
        {
            flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_HORIZONTAL);
        }

        if (sprite.flipY)
        {
            flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
        }

        return flip;
    }

    void RenderingSystem::RenderSprite(const SpriteRenderData& renderData, const CameraData& cameraData)
    {
        const TransformComponent& spriteTransform = *renderData.transform;
        const SpriteComponent& sprite = *renderData.sprite;

        const TransformComponent& cameraTransform = cameraData.transform;
        const CameraComponent& camera = cameraData.camera;

        const Sprite* spriteData = renderData.sprite->sprite;

        // pixel perfect
        SDL_Rect screenPosition;
        screenPosition.x = static_cast<int>(std::round((spriteTransform.position.x - cameraTransform.position.x) * camera.zoom));
        screenPosition.y = static_cast<int>(std::round((spriteTransform.position.y - cameraTransform.position.y) * camera.zoom));
        screenPosition.w = static_cast<int>(std::round(static_cast<float>(spriteData->Position().w) * spriteTransform.scale.x * camera.zoom));
        screenPosition.h = static_cast<int>(std::round(static_cast<float>(spriteData->Position().h) * spriteTransform.scale.y * camera.zoom));

        SDL_Point center;
        center.x = screenPosition.w / 2;
        center.y = screenPosition.h / 2;

        SDL_RendererFlip flip = FlipSprite(sprite);

        SDL_Colour colour = renderData.sprite->colour;
        spriteData->Tex().SetColour(colour.r, colour.g, colour.b);
        spriteData->Tex().SetAlpha(colour.a);

        m_Renderer.RenderSprite(spriteData->Tex(), spriteData->Position(), screenPosition, spriteTransform.angle, center, flip);
    }

}