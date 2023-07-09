#include "Scene/Systems/RenderingSystem.h"

namespace Maize {

	RenderingSystem::RenderingSystem(Renderer& renderer, AssetManager& asset) :
		m_Renderer(renderer),
        m_AssetManager(asset)
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
        std::unordered_map<std::string, std::vector<SpriteRenderData>> spriteBatches;
        GetSpriteRenderData(registry, spriteBatches);
        SortSpriteBatches(spriteBatches);

        // render each batch
        for (const auto& [texturePath, renderDataList] : spriteBatches)
        {
            const Texture& texture = *m_AssetManager.GetAsset<Texture>(texturePath);

            for (const auto& renderData : renderDataList)
            {
                RenderSprite(renderData, cameraData, texture);
            }
        }
    }

    void RenderingSystem::GetSpriteRenderData(ECS::EntityWorld& registry, std::unordered_map<std::string, std::vector<SpriteRenderData>>& spriteBatches)
    {
        spriteBatches.clear();

        for (const auto& entity : registry.GetEntityGroup<TransformComponent, SpriteComponent>())
        {
            const auto& [transform, sprite] = registry.GetComponents<TransformComponent, SpriteComponent>(entity);

            spriteBatches[sprite.texture].emplace_back(SpriteRenderData(&transform, &sprite));
        }
    }

    void RenderingSystem::SortSpriteBatches(std::unordered_map<std::string, std::vector<SpriteRenderData>>& spriteBatches)
    {
        // sort batch based on entities y position
        for (auto& [_, renderDataList] : spriteBatches)
        {
            std::ranges::sort(renderDataList.begin(), renderDataList.end(), [](const auto& a, const auto& b) {
                return a.transform->position.y < b.transform->position.y;
            });
        }
    }

    SDL_RendererFlip RenderingSystem::FlipSprite(const SpriteComponent& sprite)
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

    void RenderingSystem::RenderSprite(const SpriteRenderData& renderData, const CameraData& cameraData, const Texture& texture)
    {
        const TransformComponent& spriteTransform = *renderData.transform;
        const SpriteComponent& sprite = *renderData.sprite;

        const TransformComponent& cameraTransform = cameraData.transform;
        const CameraComponent& camera = cameraData.camera;

        SDL_Rect dstRect;
        dstRect.x = static_cast<int>(std::round((spriteTransform.position.x - cameraTransform.position.x) * camera.zoom));
        dstRect.y = static_cast<int>(std::round((spriteTransform.position.y - cameraTransform.position.y) * camera.zoom));
        dstRect.w = static_cast<int>(std::round(static_cast<float>(texture.Size().x) * spriteTransform.scale.x * camera.zoom));
        dstRect.h = static_cast<int>(std::round(static_cast<float>(texture.Size().y) * spriteTransform.scale.y * camera.zoom));

        SDL_Point center;
        center.x = dstRect.w / 2;
        center.y = dstRect.h / 2;

        SDL_RendererFlip flip = FlipSprite(sprite);

        SDL_Colour colour = renderData.sprite->colour;

        texture.SetColour(colour.r, colour.g, colour.b);
        texture.SetAlpha(colour.a);

        m_Renderer.RenderSprite(texture, dstRect, spriteTransform.angle, center, flip);
    }

}