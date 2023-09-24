#pragma once

#include <Maize.h>

#include "TilesetWindowTab.h"
#include "TilemapWindowTab.h"

namespace Maize {

    class TilemapEditorWindow
    {
    public:
		void AddComponent(TilemapComponent* tilemapComponent)
		{
			tilemapComponent->layers.emplace_back().SetName("Default");

			m_TilesetWindow.AddComponent(tilemapComponent);
			m_TilemapWindow.AddComponent(tilemapComponent);
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
		TilemapComponent* m_TilemapComponent = nullptr;

        TilesetWindowTab m_TilesetWindow;
        TilemapWindowTab m_TilemapWindow;

        TilesetSerializer m_TilesetSerializer;
        TilemapSerializer m_TilemapSerializer;
    };

} // Maize