#pragma once

#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Core/Application.h"
#include "Maize/Scene/Components.h"

namespace Maize {

    using RenderLayer = std::string;
	using SpriteData = std::vector<std::pair<int32_t, const sf::Sprite*>>;
    using SpriteBatches = std::unordered_map<RenderLayer, SpriteData>;

    class RenderingSystem
    {
    public:
        RenderingSystem() : m_Renderer(Application::Get().GetRenderer()) { }

        void OnRender(ECS::EntityWorld& registry)
        {
            RenderSprites(registry);
        }

    private:
        void RenderSprites(ECS::EntityWorld& registry)
        {
            SpriteBatches batch;
            GetDrawables(registry, batch);
            SortDrawables(batch);
            RenderSprite(batch);
        }

        void GetDrawables(ECS::EntityWorld& registry, SpriteBatches& batches) const
        {
            batches.clear();

            // sprite rendering
            for (auto entity : registry.GetEntityGroup<TransformComponent, SpriteComponent>())
            {
                const auto& [transform, sprite] = registry.GetComponents<TransformComponent, SpriteComponent>(entity);

                if (sprite.sprite == nullptr) continue;

                /*
                * TODO: add culling so it doesnt render every tile in the tilemap
                */

                sf::Sprite& spriteInfo = sprite.sprite->GetSprite();

                spriteInfo.setPosition(transform.position);
                spriteInfo.setRotation(transform.angle);
                spriteInfo.setScale(transform.scale);

                batches[sprite.sortingLayer].emplace_back( sprite.orderInLayer, &spriteInfo );
            }
        }

        static bool SortLayer(const std::pair<int32_t, const sf::Sprite*>& sprite1, const std::pair<int32_t, const sf::Sprite*>& sprite2)
        {
            // compare textures
            if (sprite1.second->getTexture() < sprite2.second->getTexture()) {
                return true;
            }
            else if (sprite1.second->getTexture() > sprite2.second->getTexture()) {
                return false;
            }

            // compare layer order if textures are the same
            return sprite1.first < sprite2.first;
        }

        void SortDrawables(SpriteBatches& batches)
        {
            for (auto& [layer, batch] : batches)
            {
                std::sort(batch.begin(), batch.end(), SortLayer);
            }
        }

        void RenderSprite(SpriteBatches& batches)
        {
            m_Renderer.BeginSceneDrawing();

            for (auto& [layer, batch] : batches)
            {
                for (auto& sprite : batch)
                {
                    m_Renderer.Draw(*sprite.second, m_Renderer.GetCurrentTexture());
                }
            }

            m_Renderer.EndSceneDrawing();
        }

    private:
        Renderer& m_Renderer;
    };

} // Maize