#pragma once

#include <Maize.h>

#include "TilesetWindowTab.h"
#include "TilemapWindowTab.h"

namespace Maize {

    class TilemapEditorWindow
    {
    public:
        explicit TilemapEditorWindow(TilemapComponent* tilemapComponent) :
            m_TilesetWindow(tilemapComponent->tilesets),
            m_TilemapWindow(tilemapComponent->tilesets, tilemapComponent->layers, tilemapComponent->tileSizeX, tilemapComponent->tileSizeY)
        {
            m_TilemapComponent->layers.emplace_back().SetName("Default");
        }

        void OnEvent(Event& e)
        {
            m_TilemapWindow.OnEvent(e);
        }

        void OnUpdate(float deltaTime)
        {
            m_TilemapWindow.OnUpdate(deltaTime);
        }

        void Window()
        {
            ImGui::Begin("Tilemap Editor", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            if (ImGui::BeginTabBar("Tabs"))
            {
                m_TilesetWindow.Window();
                m_TilemapWindow.Window();

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

    private:
		TilemapComponent* m_TilemapComponent;

        TilesetWindowTab m_TilesetWindow;
        TilemapWindowTab m_TilemapWindow;

        TilesetSerializer m_TilesetSerializer;
        TilemapSerializer m_TilemapSerializer;
    };

} // Maize