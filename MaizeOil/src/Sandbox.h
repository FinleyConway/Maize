#pragma once

#include <Maize.h>

#include "TilemapEditor/Tileset/TilesetWindow.h"

class Sandbox : public Maize::Layer
{
public:
    void OnAttach() override
    {
    }

    void OnEvent(Maize::Event& e) override
    {
        m_Camera.OnEvent(e);
    }

    void OnUpdate(float deltaTime) override
    {
        ImGui::ShowDemoWindow();

        m_Camera.OnUpdate(deltaTime);

        m_AtlasEditor.Window();
    }

    void OnRender() override
    {
    }

private:
    Maize::EditorCamera m_Camera;

    Maize::TilesetWindow m_AtlasEditor;
};