#pragma once

#include "Tileset/TilesetWindowTab.h"
#include "Tilemap/TilemapWindowTab.h"
#include "Tilemap/TilemapLayer.h"
#include "Tileset/TilesetSerializer.h"
#include "Tilemap/TilemapSerializer.h"

namespace Maize {

    struct TilemapComponent
    {
        std::vector<TilemapLayer> layers;
        std::vector<Tileset> tilesets;
        int32_t tileSizeX = 8;
        int32_t tileSizeY = 8;
    };

    class TilemapEditorWindow
    {
    public:
        TilemapEditorWindow() :
            m_TilesetWindow(m_TilemapComponent.tilesets),
            m_TilemapWindow(m_TilemapComponent.tilesets, m_TilemapComponent.layers, m_TilemapComponent.tileSizeX, m_TilemapComponent.tileSizeY)
        {
            TilemapLayer& layer0 = m_TilemapComponent.layers.emplace_back();
            layer0.SetName("Default");

            TilemapLayer& layer1 = m_TilemapComponent.layers.emplace_back();
            layer1.SetName("Middleground");

            TilemapLayer& layer2 = m_TilemapComponent.layers.emplace_back();
            layer2.SetName("Background");

            m_TilemapComponent.tilesets = m_TilesetSerializer.DeserializeTileset("tilesets.tilesets");
        }

        ~TilemapEditorWindow()
        {
            m_TilesetSerializer.SerializeTileset(m_TilemapComponent.tilesets);
            m_TilemapSerializer.SerializeTilemap(m_TilemapComponent.layers);
        }

        void OnEvent(Event& e)
        {
            m_TilemapWindow.OnEvent(e);
        }

        void OnUpdate(float deltaTime)
        {
            m_TilemapWindow.OnUpdate(deltaTime);
        }

        void OnRender()
        {
            m_TilemapWindow.OnRender();
        }

        void TilemapComponentWindow() // temp
        {
            ImGui::Begin("Tilemap Component (Temp)");

            // get tileset serial file

            // list layers that can be edited, added, remove and sorted

            // handle tilemap tile size
            int32_t tileSizeX = m_TilemapComponent.tileSizeX;
            if (ImGui::InputInt("Tile Size X", &tileSizeX))
            {
                if (tileSizeX < 1) tileSizeX = 1;
                m_TilemapComponent.tileSizeX = tileSizeX;
            }

            int32_t tileSizeY = m_TilemapComponent.tileSizeY;
            if (ImGui::InputInt("Tile Size Y", &tileSizeY))
            {
                if (tileSizeY < 1) tileSizeY = 1;
                m_TilemapComponent.tileSizeY = tileSizeY;
            }

            ImGui::End();
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

        TilesetSerializer m_TilesetSerializer;
        TilemapSerializer m_TilemapSerializer;
    };

} // Maize