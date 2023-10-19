#pragma once

#include <Maize.h>

#include "TilemapEditorWindow.h"

namespace Maize {

    enum class TilemapTools { None = 0, Pencil, Erase, Picker, Rect, Fill };

    class TilemapWindowTab
    {
    public:
        TilemapWindowTab();

        void OnEvent(Event& e);
        void Window(TilemapEditorWindow::Tilesets& tilesets, TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);

    private:
		void OnUpdate(TilemapEditorWindow::TilemapEditorGrid& editorGrid, TilemapComponent* tilemapComponent);

        void ButtonTools();
        void TilemapLayers(TilemapEditorWindow::TilemapEditorGrid& editorGrid);
        void SelectTileset(TilemapEditorWindow::Tilesets& tilesets);
        void SelectTile();

        bool OnKeyPressed(const KeyPressedEvent& e);
        bool OnMousePressed(const MouseButtonPressedEvent& e);
        bool OnMouseReleased(const MouseButtonReleasedEvent& e);

    private:
        bool m_MouseLeftHeld = false;

        Tileset* m_SelectedTileset = nullptr;
        TilemapEditorTile m_SelectedTile;
        uint32_t m_SelectedTilemapLayer = 0;
		float m_TilesetZoomFactor = 4.0f;

        bool m_FlipTileX = false;
        bool m_FlipTileY = false;
        float m_CurrentRotation = 0.0f;
        TilemapTools m_CurrentTool = TilemapTools::Pencil;

        std::shared_ptr<Texture> m_IconPencil, m_IconEraser, m_IconPicker;
    };

} // Maize