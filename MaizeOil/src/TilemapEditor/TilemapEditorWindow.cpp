#include "TilemapEditorWindow.h"

namespace Maize {

	void TilemapEditorWindow::AddComponent(TilemapComponent* tilemapComponent)
	{
		m_TilemapComponent = tilemapComponent;
		tilemapComponent->tilemapLayers.emplace_back();
	}

	void TilemapEditorWindow::OnEvent(Event& e)
	{
		m_TilemapWindow.OnEvent(e);
	}

	void TilemapEditorWindow::Window()
	{
		ImGui::Begin("Tilemap Editor", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

		if (ImGui::BeginTabBar("Tabs"))
		{
			if (m_TilemapComponent != nullptr)
			{
				m_TilesetWindow.Window(m_Tilesets, m_EditorGrids, m_TilemapComponent);
				m_TilemapWindow.Window(m_Tilesets, m_EditorGrids, m_TilemapComponent);
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

}
