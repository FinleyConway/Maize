#pragma once

#include <format>

#include "../Tileset/Tileset.h"
#include "../../CartesianGrid.h"

namespace Maize {

    class TilemapWindowTab
    {
    public:
        explicit TilemapWindowTab(std::vector<Tileset>& tilesets);

        void OnEvent(Event& e);
        void Window();

    private:
        void ButtonTools();
        void SelectTileset();
        void SelectTile();
        void SetSelectedTile(int32_t tilesetID, int32_t index);
        bool OnKeyPressed(const KeyPressedEvent& e);

    private:
        std::vector<Tileset>& m_Tilesets;

        Tileset* m_SelectedTileset = nullptr;
        TilemapTile m_SelectedTile;

        enum class Tools { None = 0, Pencil, Erase, Picker, Rect, Fill };

        bool m_FlipTileX = false;
        bool m_FlipTileY = false;
        float m_CurrentRotation = 0.0f;
        Tools m_CurrentTool = Tools::Pencil;

        std::shared_ptr<Texture> m_IconPencil, m_IconEraser, m_IconPicker, m_IconFill, m_IconRect;
    };

} // Maize