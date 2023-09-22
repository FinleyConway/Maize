#pragma once

#include <Maize.h>

namespace Maize {

    enum class TilemapTools { None = 0, Pencil, Erase, Picker, Rect, Fill };

    class TilemapWindowTab
    {
    public:
        TilemapWindowTab(std::vector<Tileset>& tilesets, std::vector<TilemapLayer>& tilemapLayers, int32_t& cellSizeX, int32_t& cellSizeY);

        void OnEvent(Event& e);
        void OnUpdate(float deltaTime);
        void Window();

    private:
        void ButtonTools();
        void TilemapLayers();
        void SelectTileset();
        void SelectTile();
        void SetSelectedTile(int32_t tilesetID, int32_t index);

        bool OnKeyPressed(const KeyPressedEvent& e);
        bool OnMousePressed(const MouseButtonPressedEvent& e);
        bool OnMouseReleased(const MouseButtonReleasedEvent& e);

    private:
        std::vector<Tileset>& m_Tilesets;
        std::vector<TilemapLayer>& m_TilemapLayers;
        int32_t& m_CellSizeX;
        int32_t& m_CellSizeY;

        sf::Vector2f m_MousePosition;
        bool m_MouseLeftHeld = false;

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