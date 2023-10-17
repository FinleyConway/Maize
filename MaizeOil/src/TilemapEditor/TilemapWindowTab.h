#pragma once

#include <Maize.h>

namespace Maize {

    enum class TilemapTools { None = 0, Pencil, Erase, Picker, Rect, Fill };

    class TilemapWindowTab
    {
    public:
        TilemapWindowTab();

		void AddComponent(TilemapComponent* tilemapComponent)
		{
			m_TilemapComponent = tilemapComponent;
		}
        void OnEvent(Event& e);
        void Window(float deltaTime);

    private:
		void OnUpdate(float deltaTime);

        void ButtonTools();
        void TilemapLayers();
        void SelectTileset();
        void SelectTile();

        bool OnKeyPressed(const KeyPressedEvent& e);
        bool OnMousePressed(const MouseButtonPressedEvent& e);
        bool OnMouseReleased(const MouseButtonReleasedEvent& e);

    private:
		TilemapComponent* m_TilemapComponent = nullptr;

        bool m_MouseLeftHeld = false;

        Tileset* m_SelectedTileset = nullptr;
        TilemapTile m_SelectedTile;
        uint32_t m_SelectedTilemapLayer = 0;
		float m_TilesetZoomFactor = 4.0f;

        bool m_FlipTileX = false;
        bool m_FlipTileY = false;
        float m_CurrentRotation = 0.0f;
        TilemapTools m_CurrentTool = TilemapTools::Pencil;

        std::shared_ptr<Texture> m_IconPencil, m_IconEraser, m_IconPicker, m_IconFill, m_IconRect;
    };

} // Maize