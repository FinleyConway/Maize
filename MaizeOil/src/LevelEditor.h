#pragma once

#include <Maize.h>

#include "TilemapEditor/TilemapEditorWindow.h"
#include "Maize/Utils/Grids/CartesianGrid.h"

class LevelEditor : public Maize::Layer
{
public:
    LevelEditor()
    {
        auto tilemapE = m_Reg.CreateEntity();
        m_Reg.AddComponent<Maize::TransformComponent>(tilemapE);
        m_TilemapComponent = &m_Reg.AddComponent<Maize::TilemapComponent>(tilemapE);
		m_TilemapEditorWindow.AddComponent(m_TilemapComponent);

        m_Grid.setPrimitiveType(sf::Quads);
        m_Grid.resize(1000 * 1000);
    }

    void OnEvent(Maize::Event& e) override
    {
        m_Camera.OnEvent(e);

        m_TilemapEditorWindow.OnEvent(e);
    }

    void OnUpdate(float deltaTime) override
    {
        ImGui::ShowDemoWindow();

        m_Camera.OnUpdate(deltaTime);

		if (m_TilemapComponent != nullptr)
		{
			m_TilemapEditorWindow.OnUpdate(deltaTime);
			m_TilemapEditorWindow.Window();

			if (ImGui::Button("Generate"))
			{
				std::vector<Maize::TextureInfo> textureInfo;

				textureInfo.reserve(m_TilemapComponent->tilesets.size());
                for (auto& [tilesetID, tileset] : m_TilemapComponent->tilesets)
				{
                    textureInfo.emplace_back(*tileset.GetTexture(), tilesetID);
				}

				Maize::TexturePacker pack;
				auto results = pack.Pack(textureInfo, sf::Vector2u(1024, 1024));

                m_Texture = results.packedTexture;

                /*
                 * BUG: drawing to tilemap then adding a new texture which get packed doesnt show the same texture
                */
                for (auto& [tilesetID, tileset] : m_TilemapComponent->tilesets)
                {
                    for (auto& [id, rect] : results.textureInfo)
                    {
                        if (id == tilesetID)
                        {
                            for (auto& [tileID, tile] : tileset.GetTiles())
                            {
                                tile.texCoords.x += rect.getPosition().x;
                                tile.texCoords.y += rect.getPosition().y;
                            }
                        }
                    }
                }
			}
		}

		//m_RenderingSystem.OnRender(m_Reg);

        auto& ren = Maize::Application::Get().GetRenderer();

        ren.BeginSceneDrawing();

        for (const auto& layer : m_TilemapComponent->layers)
        {
            sf::Vector2i halfSize = layer.GetGridSize() / 2;

            for (int32_t x = -halfSize.x; x < halfSize.x; x++)
            {
                for (int32_t y = -halfSize.y; y < halfSize.y; y++)
                {
                    const Maize::TilemapTile& tile = layer.GetTile(sf::Vector2i(x, y));

                    if (tile.IsValid()) // invalid texture coord
                    {
                        sf::Vector2i texturePos = tile.texCoords;
                        int32_t tileSizeX = m_TilemapComponent->tileSizeX;
                        int32_t tileSizeY = m_TilemapComponent->tileSizeY;

                        sf::Vertex quad[4];
                        quad[0].position = sf::Vector2f(x * tileSizeX, y * tileSizeY);
                        quad[1].position = sf::Vector2f((x + 1) * tileSizeX, y * tileSizeY);
                        quad[2].position = sf::Vector2f((x + 1) * tileSizeX, (y + 1) * tileSizeY);
                        quad[3].position = sf::Vector2f(x * tileSizeX, (y + 1) * tileSizeY);

                        quad[0].texCoords = sf::Vector2f(texturePos.x * tileSizeX, texturePos.y * tileSizeY);
                        quad[1].texCoords = sf::Vector2f((texturePos.x + 1) * tileSizeX, texturePos.y * tileSizeY);
                        quad[2].texCoords = sf::Vector2f((texturePos.x + 1) * tileSizeX, (texturePos.y + 1) * tileSizeY);
                        quad[3].texCoords = sf::Vector2f(texturePos.x * tileSizeX, (texturePos.y + 1) * tileSizeY);

                        int32_t index = (x + halfSize.x) * 4 + (y + halfSize.y) * layer.GetGridSize().x * 4;
                        for (int32_t i = 0; i < 4; i++)
                        {
                            m_Grid[index + i] = quad[i];
                        }
                    }
                }
            }
        }

        ren.Draw(m_Grid, &m_Texture, nullptr);

        ren.EndSceneDrawing();
    }

private:
    ECS::EntityWorld m_Reg;

    Maize::EditorCamera m_Camera;

    Maize::TilemapComponent* m_TilemapComponent;
    Maize::TilemapEditorWindow m_TilemapEditorWindow;

	Maize::RenderingSystem m_RenderingSystem;

	sf::Texture m_Texture;
    sf::VertexArray m_Grid;
	sf::Vector2i m_GridSize;
};