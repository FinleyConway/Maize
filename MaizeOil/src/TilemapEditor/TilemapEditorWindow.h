#pragma once

#include "Tileset/TilesetWindowTab.h"
#include "Tilemap/TilemapWindowTab.h"
#include "Tilemap/TilemapLayer.h"

namespace Maize {

    struct TilemapComponent
    {
        std::vector<TilemapLayer> layers;
        std::vector<Tileset> tilesets;
        int32_t tileSizeX = 16;
        int32_t tileSizeY = 16;

        TilemapLayer& GetMap(uint32_t index)
        {
            return layers[index];
        }
    };

    class TilemapEditorWindow
    {
    public:
        TilemapEditorWindow() :
            m_TilesetWindow(m_TilemapComponent.tilesets),
            m_TilemapWindow(m_TilemapComponent.tilesets, m_TilemapComponent.layers)
        {
            TilemapLayer& layer0 = m_TilemapComponent.layers.emplace_back();
            layer0.SetName("Default");

            TilemapLayer& layer1 = m_TilemapComponent.layers.emplace_back();
            layer1.SetName("Middleground");

            TilemapLayer& layer2 = m_TilemapComponent.layers.emplace_back();
            layer2.SetName("Background");
        }

        void OnEvent(Event& e)
        {
            m_TilemapWindow.OnEvent(e);
        }

        void Window()
        {
            bool open = true;
            ImGui::Begin("Tilemap Editor", &open, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

            if (ImGui::BeginTabBar("Tabs"))
            {
                m_TilesetWindow.Window();
                m_TilemapWindow.Window();

                ImGui::EndTabBar();
            }

            ImGui::End();
        }

    private:
        TilesetWindowTab m_TilesetWindow;
        TilemapWindowTab m_TilemapWindow;

        TilemapComponent m_TilemapComponent;
    };

} // Maize