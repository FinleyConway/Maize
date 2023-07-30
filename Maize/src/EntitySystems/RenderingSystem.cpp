#include "EntitySystems/RenderingSystem.h"

namespace Maize {

	void RenderingSystem::OnRender(ECS::EntityWorld& registry, float dt)
    {
        // iterate through the camera entities
        for (const auto& cameraEntity : registry.GetEntityGroup<TransformComponent, CameraComponent>())
        {
            const auto& [cameraTransform, camera] = registry.GetComponents<TransformComponent, CameraComponent>(cameraEntity);

            RenderSprites(registry, CameraData(cameraTransform, camera));
        }
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

            // only add valid and visible sprites to the batch
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
        const Point& spriteSize = Point(renderData.sprite->sprite->Position().width, renderData.sprite->sprite->Position().height);
        const float spriteRotation = renderData.transform->angle;

        const PointF& cameraPosition = cameraData.transform.position;
        const Point& cameraBounds = cameraData.camera.bounds;
        const float cameraSize = cameraData.camera.size;
        const float cameraSizeFactor = cameraBounds.y / cameraSize;

        // Transform sprite position based on rotation
        const PointF rotatedSpritePosition = RotateVector(spritePosition, spriteRotation);

        // Transform sprite size based on rotation
        const Point rotatedSpriteSize(
            abs(static_cast<int>(spriteSize.x * cos(spriteRotation))) + abs(static_cast<int>(spriteSize.y * sin(spriteRotation))),
            abs(static_cast<int>(spriteSize.x * sin(spriteRotation))) + abs(static_cast<int>(spriteSize.y * cos(spriteRotation)))
        );

        const float spriteRight = rotatedSpritePosition.x + static_cast<float>(rotatedSpriteSize.x);
        const float spriteBottom = rotatedSpritePosition.y + static_cast<float>(rotatedSpriteSize.y);
        const float cameraRight = cameraPosition.x + cameraBounds.x / cameraSizeFactor;
        const float cameraBottom = cameraPosition.y + cameraBounds.y / cameraSizeFactor;

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

        float meter = 1;
        float spriteScaleFactor = meter / spriteData->PPU();

        Point offset = camera.viewport.Size() / 2 - spriteData->Position().Size() / 2;

        // calculate the scale factor based on camera size
        float cameraSizeFactor = cameraData.camera.bounds.y / cameraData.camera.size;

        // calculate the sprite screen position and size
        Rect spriteScreenPos;
        spriteScreenPos.x = static_cast<int32_t>((spriteTransform.position.x - cameraTransform.position.x) * cameraSizeFactor) + offset.x;
        spriteScreenPos.y = static_cast<int32_t>((spriteTransform.position.y - cameraTransform.position.y) * cameraSizeFactor) + offset.y;
        spriteScreenPos.width = static_cast<int32_t>(static_cast<float>(spriteData->Position().width) * spriteTransform.scale.x * cameraSizeFactor * spriteScaleFactor);
        spriteScreenPos.height = static_cast<int32_t>(static_cast<float>(spriteData->Position().height) * spriteTransform.scale.y * cameraSizeFactor * spriteScaleFactor);

        // adjust the position to keep the sprite centered
        spriteScreenPos.x -= spriteScreenPos.width / 2;
        spriteScreenPos.y -= spriteScreenPos.height / 2;

        SDL_RendererFlip flip = FlipSprite(sprite);

        // set texture properties
        Colour colour = renderData.sprite->colour;
        spriteData->Tex().SetColour(colour.r, colour.g, colour.b);
        spriteData->Tex().SetAlpha(colour.a);

        Renderer::RenderSprite(spriteData->Tex(), spriteData->Position(), spriteScreenPos, spriteTransform.angle, spriteData->Pivot(), flip);
    }

}