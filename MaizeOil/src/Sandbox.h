#pragma once

#include <Maize.h>

#include "TilemapEditor/TilemapWindow.h"
#include "TilemapEditor/Tileset/TilesetWindow.h"

class Sandbox : public Maize::Layer
{
public:
    void OnAttach() override
    {
        m_TilemapWindow.OnAttach();
    }

    void OnEvent(Maize::Event& e) override
    {
        m_Camera.OnEvent(e);

        m_TilemapWindow.OnEvent(e);
    }

    void OnUpdate(float deltaTime) override
    {
        ImGui::ShowDemoWindow();

        m_Camera.OnUpdate(deltaTime);
        m_TilemapWindow.OnUpdate(deltaTime);

        m_AtlasEditor.Window();
    }

    void OnRender() override
    {
        m_TilemapWindow.OnRender();
    }

private:
    Maize::TilemapWindow m_TilemapWindow;
    Maize::EditorCamera m_Camera;

    Maize::TilesetWindow m_AtlasEditor;
};