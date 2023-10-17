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
		m_TilemapComponent->layers.emplace_back().SetName("Background");

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
			m_TilemapEditorWindow.Window(deltaTime);
		}

		//m_RenderingSystem.OnRender(m_Reg);

        auto& ren = Maize::Application::Get().GetRenderer();

        ren.BeginSceneDrawing();

		if (m_TilemapComponent->texture != nullptr)
		{
			for (auto& layer : m_TilemapComponent->layers)
			{
				auto halfSize = layer.GetGridSize() / 2;

				for (int y = -halfSize.y; y < halfSize.y; y++)
				{
					for (int x = -halfSize.x; x < halfSize.x; x++)
					{
						auto& tile = layer.GetTile({ x, y });

						if (!tile.IsValid()) continue;

						std::cout << tile.texCoords.x << " " << tile.texCoords.y << std::endl;
					}
				}

				ren.Draw(layer.GetGridRenderer().GetGrid(), m_TilemapComponent->texture.get());
			}
		}

        ren.EndSceneDrawing();
    }

private:
    ECS::EntityWorld m_Reg;

    Maize::EditorCamera m_Camera;

    Maize::TilemapComponent* m_TilemapComponent;
    Maize::TilemapEditorWindow m_TilemapEditorWindow;

	Maize::RenderingSystem m_RenderingSystem;
};