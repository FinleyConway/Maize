#pragma once

#include <Maize.h>

#include "TilemapEditor/TilemapEditorWindow.h"

class LevelEditor : public Maize::Layer
{
public:
    LevelEditor() : m_Pack({512, 512})
    {
        auto tilemapE = m_Reg.CreateEntity();
        m_Reg.AddComponent<Maize::TransformComponent>(tilemapE);
        m_TilemapComponent = &m_Reg.AddComponent<Maize::TilemapComponent>(tilemapE);
		m_TilemapEditorWindow.AddComponent(m_TilemapComponent);

        std::vector<sf::IntRect> rects;

        rects.emplace_back(0, 0, 32, 32);
        rects.emplace_back(0, 0, 16, 16);
        rects.emplace_back(0, 0, 8, 8);
        rects.emplace_back(0, 0, 128, 128);
        rects.emplace_back(0, 0, 128, 128);
        rects.emplace_back(0, 0, 128, 128);
        rects.emplace_back(0, 0, 128, 128);

        m_Pack.Pack(rects);
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
		}

		//m_RenderingSystem.OnRender(m_Reg);

        auto& ren = Maize::Application::Get().GetRenderer();

        ren.BeginSceneDrawing();

        sf::Sprite sprite(m_Pack.GetTexture());
        ren.Draw(sprite, ren.GetCurrentTexture());

        ren.EndSceneDrawing();
    }

private:
    ECS::EntityWorld m_Reg;

    Maize::EditorCamera m_Camera;

    Maize::TilemapComponent* m_TilemapComponent;
    Maize::TilemapEditorWindow m_TilemapEditorWindow;

	Maize::RenderingSystem m_RenderingSystem;
    Maize::TexturePacking m_Pack;


};