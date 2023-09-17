#pragma once

#include <Maize.h>

#include "TilemapEditor/TilemapEditorWindow.h"

class Sandbox : public Maize::Layer
{
public:
    void OnAttach() override
    {
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

        m_TilemapEditorWindow.OnUpdate(deltaTime);
        m_TilemapEditorWindow.Window();
    }

    void OnRender() override
    {
        m_TilemapEditorWindow.OnRender();
    }

private:
    Maize::EditorCamera m_Camera;

    Maize::TilemapEditorWindow m_TilemapEditorWindow;
};