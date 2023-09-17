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
            m_TilemapComponent.layers.emplace_back().SetName("Default");
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
		TilemapComponent m_TilemapComponent;

        TilesetWindowTab m_TilesetWindow;
        TilemapWindowTab m_TilemapWindow;

        TilesetSerializer m_TilesetSerializer;
        TilemapSerializer m_TilemapSerializer;
    };

} // Maize