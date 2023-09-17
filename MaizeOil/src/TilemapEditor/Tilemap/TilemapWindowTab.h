#pragma once

#include "../Tileset/Tileset.h"
#include "../../CartesianGrid.h"
#include "TilemapLayer.h"

namespace Maize {

    enum class TilemapTools { None = 0, Pencil, Erase, Picker, Rect, Fill };

    class TilemapWindowTab
    {
    public:
        TilemapWindowTab(std::vector<Tileset>& tilesets, std::vector<TilemapLayer>& tilemapLayers, int32_t& cellSizeX, int32_t& cellSizeY);

        void OnEvent(Event& e);
        void OnUpdate(float deltaTime);
        void OnRender() // temp
        {
            // very cursed and will make it better performant
            for (auto &mapLayers: m_TilemapLayers)
            {
                Point gridSize = mapLayers.GetGridSize();
                int32_t halfWidth = gridSize.x / 2;
                int32_t halfHeight = gridSize.y / 2;

                for (int32_t y = -halfHeight; y < halfHeight; y++)
                {
                    for (int32_t x = -halfWidth; x < halfWidth; x++)
                    {
                        const TilemapTile &tilemapTile = mapLayers.GetTile(Point(x, y));

                        if (tilemapTile.IsValid())
                        {
                            Tile *tile = Tileset::FindTileByTilesetID(m_Tilesets, tilemapTile.tilesetID,
                                                                      tilemapTile.index);

                            if (tile == nullptr) continue;

                            Sprite &sprite = tile->GetSprite();
                            PointF screenPosition =
                                    CartesianGrid::ConvertGridToScreen(Point(x, y), m_CellSizeX, m_CellSizeY) +
                                    sprite.GetPivot();

                            sprite.SetPosition(screenPosition);
                            sprite.FlipX(tilemapTile.flipX);
                            sprite.FlipY(tilemapTile.flipY);
                            sprite.SetAngle(tilemapTile.rotation);

                            Application::Get().GetWindow().Render(sprite);
                        }
                    }
                }
            }
        }
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

        PointF m_MousePosition;
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