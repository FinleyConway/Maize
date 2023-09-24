#pragma once

#include <Maize.h>

#include "TilemapEditor/TilemapEditorWindow.h"

class LevelEditor : public Maize::Layer
{
public:
    LevelEditor()
    {
        auto ent = m_Reg.CreateEntity();
        m_Reg.AddComponent<Maize::TransformComponent>(ent);
        m_Reg.AddComponent<Maize::TilemapComponent>(ent);

        m_TilemapComponent = &m_Reg.GetComponent<Maize::TilemapComponent>(ent);

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
		}

		m_RenderingSystem.OnRender(m_Reg);
    }

private:
    ECS::EntityWorld m_Reg;

    Maize::EditorCamera m_Camera;

    Maize::TilemapComponent* m_TilemapComponent;
    Maize::TilemapEditorWindow m_TilemapEditorWindow;

	Maize::RenderingSystem m_RenderingSystem;
};