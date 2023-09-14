#pragma once

#include "../Tileset/Tileset.h"
#include "../../CartesianGrid.h"
#include "TilemapLayer.h"

namespace Maize {

    enum class TilemapTools { None = 0, Pencil, Erase, Picker, Rect, Fill };

    class TilemapWindowTab
    {
    public:
        TilemapWindowTab(std::vector<Tileset>& tilesets, std::vector<TilemapLayer>& tilemapLayers);

        void OnEvent(Event& e);
        void Window();

        const TilemapTile& GetSelectedTile() const { return m_SelectedTile; }
        uint32_t GetSelectedLayer() const { return m_SelectedTilemapLayer; }
        bool GetTileFlippedX() const { return m_FlipTileX; }
        bool GetTileFlippedY() const { return m_FlipTileY; }
        float GetCurrentRotation() const { return m_CurrentRotation; }
        TilemapTools GetSelectedTool() const { return m_CurrentTool; }

    private:
        void ButtonTools();
        void TilemapLayers();
        void SelectTileset();
        void SelectTile();
        void SetSelectedTile(int32_t tilesetID, int32_t index);
        bool OnKeyPressed(const KeyPressedEvent& e);

    private:
        std::vector<Tileset>& m_Tilesets;
        std::vector<TilemapLayer>& m_TilemapLayers;

        Tileset* m_SelectedTileset = nullptr;
        TilemapTile m_SelectedTile;
        uint32_t m_SelectedTilemapLayer = 0;

        bool m_FlipTileX = false;
        bool m_FlipTileY = false;
        float m_CurrentRotation = 0.0f;
        TilemapTools m_CurrentTool = TilemapTools::Pencil;

        std::shared_ptr<Texture> m_IconPencil, m_IconEraser, m_IconPicker, m_IconFill, m_IconRect;
    };

} // Maize