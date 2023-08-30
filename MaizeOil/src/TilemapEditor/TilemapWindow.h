#pragma once

#include <Maize.h>

#include "Tilemap/Tilemap.h"
#include "Tilemap/TilemapSerializer.h"

#include "Tileset/TilesetManager.h"

namespace Maize {

    enum class TileMapTool
    {
        None, Paint, Erase, Picker, Fill, Rect
    };

    class TilemapWindow
    {
    public:
        void OnAttach();
        void OnEvent(Event& e);
        void OnUpdate(float deltaTime);
        void OnRender();

    private:
        void TilesetWindow();
        void TilemapButtonTools();
        void SelectLayer();
        void SelectTileset();
        void TileSelector(bool isWindowHovered);

        bool OnKeyPressed(const KeyPressedEvent& e);
        bool OnMouseButtonDown(const MouseButtonPressedEvent& e);
        bool OnMouseButtonUp(const MouseButtonReleasedEvent& e);

    private:
        Tilemap m_TileMap;
        TilesetManager m_TilesetManager;

        TilemapSerializer m_Serializer;

        PointF m_MousePosition;
        PointF m_PressedMousePosition;
        bool m_MouseButtonPressed = false;
        bool m_MouseButtonHeld = false;
        TileMapTool m_CurrentTool = TileMapTool::Paint;

        std::shared_ptr<Texture> m_IconPencil, m_IconEraser, m_IconPicker, m_IconFill, m_IconRect;

        bool m_IsSaved = true;
    };

} // Maize