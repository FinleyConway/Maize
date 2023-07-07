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

        std::unordered_map<std::string, std::vector<TransformComponent>> batch;
        CreateBatches(registry, batch);

        // iterate through the camera entities
        for (const auto& cameraEntity : registry.GetEntityGroup<TransformComponent, CameraComponent>())
        {
            const auto& [cameraTransform, camera] = registry.GetComponents<TransformComponent, CameraComponent>(cameraEntity);
            CameraGroup cameraGroup(cameraTransform, camera);

            // render each batch
            for (const auto& [texturePath, transforms] : batch)
            {
                const Texture& texture = *m_AssetManager.GetAsset<Texture>(texturePath);

                for (const auto& transform : transforms)
                {
                    SpriteGroup spriteGroup(transform, texture);
                    RenderSprite(spriteGroup, cameraGroup);
                }
            }
        }

        m_Renderer.Present();
    }

    void RenderingSystem::CreateBatches(ECS::EntityWorld& registry, auto& batch)
    {
        batch.clear();

        // Collect entities with required components into batches based on texture path
        for (auto& entity : registry.GetEntityGroup<TransformComponent, SpriteComponent>())
        {
            const auto& [transform, sprite] = registry.GetComponents<TransformComponent, SpriteComponent>(entity);
            batch[sprite.texture].push_back(transform);
        }
    }

    void RenderingSystem::RenderSprite(const SpriteGroup& spriteGroup, const CameraGroup& cameraGroup)
    {
        SDL_Rect dstRect;
        dstRect.x = static_cast<int>(std::round((spriteGroup.transform.position.x - cameraGroup.transform.position.x) * cameraGroup.camera.zoom));
        dstRect.y = static_cast<int>(std::round((spriteGroup.transform.position.y - cameraGroup.transform.position.y) * cameraGroup.camera.zoom));
        dstRect.w = static_cast<int>(std::round(static_cast<float>(spriteGroup.texture.Size().x) * spriteGroup.transform.scale.x * cameraGroup.camera.zoom));
        dstRect.h = static_cast<int>(std::round(static_cast<float>(spriteGroup.texture.Size().y) * spriteGroup.transform.scale.y * cameraGroup.camera.zoom));

        SDL_Point center;
        center.x = dstRect.w / 2;
        center.y = dstRect.h / 2;

        m_Renderer.RenderSprite(spriteGroup.texture, dstRect, spriteGroup.transform.angle, center);
    }

}