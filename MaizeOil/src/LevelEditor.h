#pragma once

#include <Maize.h>

#include "TilemapEditor/TilemapEditorWindow.h"

class LevelEditor : public Maize::Layer
{
public:
    LevelEditor()
    {
        auto tilemapE = m_Reg.CreateEntity();
        m_Reg.AddComponent<Maize::TransformComponent>(tilemapE);
        m_TilemapComponent = &m_Reg.AddComponent<Maize::TilemapComponent>(tilemapE);
		m_TilemapEditorWindow.AddComponent(m_TilemapComponent);
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

                for (auto& [tilesetID, tileset] : m_TilemapComponent->tilesets)
                {
                    for (auto& [id, rect] : results.textureInfo)
                    {
                        if (id == tilesetID)
                        {
                            for (auto& [tileID, tile] : tileset.GetTiles())
                            {
                                tile.position += rect.getPosition();
                            }
                        }
                    }
                }
			}
		}

		//m_RenderingSystem.OnRender(m_Reg);

        auto& ren = Maize::Application::Get().GetRenderer();

        ren.BeginSceneDrawing();

        sf::Sprite sprite(m_Texture);
        ren.Draw(sprite);

        ren.EndSceneDrawing();
    }

private:
    ECS::EntityWorld m_Reg;

    Maize::EditorCamera m_Camera;

    Maize::TilemapComponent* m_TilemapComponent;
    Maize::TilemapEditorWindow m_TilemapEditorWindow;

	Maize::RenderingSystem m_RenderingSystem;
	sf::Texture m_Texture;
};