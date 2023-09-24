#pragma once

#include <EntityComponentSystem/EntityWorld.h>

#include "Maize/Core/Application.h"
#include "Maize/Scene/Components.h"

namespace Maize {

    using RenderLayer = std::string;
    using SpriteBatches = std::unordered_map<RenderLayer, std::vector<std::pair<int32_t, const sf::Sprite*>>>;

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

                batches.at(sprite.sortingLayer).emplace_back(sprite.orderInLayer, &spriteInfo);
            }

            // tilemap rendering
            for (auto entity : registry.GetEntityGroup<TransformComponent, TilemapComponent>())
            {
                const auto& [transform, tilemap] = registry.GetComponents<TransformComponent, TilemapComponent>(entity);

                for (const auto& map : tilemap.layers)
                {
                    sf::Vector2i gridSize = map.GetGridSize();
                    sf::Vector2i halfSize = gridSize / 2;

                    for (int32_t y = -halfSize.y; y < halfSize.y; y++)
                    {
                        for (int32_t x = -halfSize.x; x < halfSize.x; x++)
                        {
                            const TilemapTile &tilemapTile = map.GetTile(sf::Vector2i(x, y));

                            if (!tilemapTile.IsValid()) continue;

                            Tile* tile = Tileset::FindTileByTilesetID(tilemap.tilesets, tilemapTile.tilesetID,
                                                                      tilemapTile.index);

                            if (tile == nullptr) continue;

                            sf::Sprite& sprite = tile->GetSprite().GetSprite();

                            /*
                             * TODO: add culling so it doesnt render every tile in the tilemap
                             */

                            sf::Vector2f screenPosition =
                                    CartesianGrid::ConvertGridToScreen(sf::Vector2i(x, y), tilemap.tileSizeX,
                                                                       tilemap.tileSizeY) + sprite.getOrigin();

                            screenPosition += transform.position;

                            sprite.setPosition(screenPosition);
                            tile->GetSprite().FlipX(tilemapTile.flipX);
                            tile->GetSprite().FlipX(tilemapTile.flipY);
                            sprite.setRotation(tilemapTile.rotation + transform.angle);
                            sprite.scale(transform.scale);

                            batches.at(tilemap.sortingLayer).emplace_back(tilemap.orderInLayer, &sprite);
                        }
                    }
                }
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
                    m_Renderer.Draw(*sprite.second);
                }
            }

            m_Renderer.EndSceneDrawing();
        }

    private:
        Renderer& m_Renderer;
    };

} // Maize