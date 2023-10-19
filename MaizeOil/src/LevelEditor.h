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
			m_TilemapEditorWindow.Window();
		}

		//m_RenderingSystem.OnRender(m_Reg);

        auto& ren = Maize::Application::Get().GetRenderer();

        ren.BeginSceneDrawing();

        if (m_TilemapComponent->tilemapTexture)
        {
            sf::Sprite sprite(*m_TilemapComponent->tilemapTexture);

            ren.Draw(sprite);
        }

		if (m_TilemapComponent->tilemapTexture != nullptr)
		{
			for (auto& layer : m_TilemapComponent->tilemapLayers)
			{
				ren.Draw(layer.GetVertexArray(), m_TilemapComponent->tilemapTexture.get());
			}
		}

        ren.EndSceneDrawing();
    }

private:
    ECS::EntityWorld m_Reg;

    Maize::EditorCamera m_Camera;

    Maize::TilemapComponent* m_TilemapComponent = nullptr;
    Maize::TilemapEditorWindow m_TilemapEditorWindow;

	Maize::RenderingSystem m_RenderingSystem;
};