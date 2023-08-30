#include "TilemapWindow.h"

namespace Maize {

    void TilemapWindow::OnAttach()
    {
        m_IconPencil = Texture::Create("Resources/Icons/pencil.png");
        m_IconEraser = Texture::Create("Resources/Icons/eraser.png");
        m_IconPicker = Texture::Create("Resources/Icons/color-picker.png");
        m_IconFill = Texture::Create("Resources/Icons/fill.png");
        m_IconRect = Texture::Create("Resources/Icons/rect.png");

        m_TilesetManager.LoadTileset("Saves/Test0.tileset");
        m_TilesetManager.LoadTileset("Saves/Test1.tileset");

        auto tilemap = m_Serializer.DeserializeTilemap("Saves/TestMapName.tilemap");
        if (tilemap.has_value())
        {
            m_TileMap.AddLayer("Default", tilemap.value());
            m_TileMap.SetCurrentLayer("Default");
        }
    }

    void TilemapWindow::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(std::bind(&TilemapWindow::OnKeyPressed, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&TilemapWindow::OnMouseButtonDown, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&TilemapWindow::OnMouseButtonUp, this, std::placeholders::_1));

        m_TileMap.OnEvent(e);
    }

    void TilemapWindow::OnUpdate(float deltaTime)
    {
        m_MousePosition = Camera::ScreenToWorld(Input::GetMousePosition());
        Point mouseGridPosition = m_TileMap.ConvertScreenToGrid(m_MousePosition);

        if (m_MouseButtonHeld)
        {
            if (m_CurrentTool == TileMapTool::Paint)
            {
                m_TileMap.PlaceTile(mouseGridPosition);
            }
            else if (m_CurrentTool == TileMapTool::Erase)
            {
                m_TileMap.RemoveTile(mouseGridPosition);
            }
            else if (m_CurrentTool == TileMapTool::Picker)
            {
                m_TileMap.GetTileInfo(mouseGridPosition);
                m_CurrentTool = TileMapTool::Paint;
            }
            else if (m_CurrentTool == TileMapTool::Fill)
            {
                m_TileMap.FillTiles(mouseGridPosition, m_TileMap.GetTile(mouseGridPosition));
                m_IsSaved = false;
            }
        }

        // imgui update
        TilesetWindow();
    }

    void TilemapWindow::OnRender()
    {
        Point gridSize = m_TileMap.GetGridSize("Default");
        int32_t halfWidth = gridSize.x / 2;
        int32_t halfHeight = gridSize.y / 2;

        // very cursed and will make it better performant
        for (int32_t y = -halfHeight; y < halfHeight; y++)
        {
            for (int32_t x = -halfWidth; x < halfWidth; x++)
            {
                const TileMapTile& tilemapTile = m_TileMap.GetTile(Point(x, y), "Default");

                if (tilemapTile.IsValid())
                {
                    Tile& tile = m_TilesetManager.GetTilesets().at(tilemapTile.tilesetID).GetTile(tilemapTile.index);

                    Sprite& sprite = tile.GetSprite();
                    PointF screenPosition = m_TileMap.ConvertGridToScreen(Point(x, y)) + sprite.GetPivot();

                    sprite.SetPosition(screenPosition);
                    sprite.FlipX(tilemapTile.flipX);
                    sprite.FlipY(tilemapTile.flipY);
                    sprite.SetAngle(tilemapTile.rotation);

                    Application::Get().GetWindow().Render(sprite);
                }
            }
        }
    }

    void TilemapWindow::TilesetWindow()
    {
        ImGui::Begin("Tilesets");

        bool isWindowHovered = ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows);

        if (isWindowHovered)
        {
            ImGui::SetWindowFocus(nullptr);
        }

        TilemapButtonTools();
        SelectLayer();
        SelectTileset();

        if (m_TilesetManager.GetSelectedTileset() != nullptr)
        {
            TileSelector(isWindowHovered);
        }

        ImGui::End();
    }

    void TilemapWindow::TilemapButtonTools()
    {
        const PointF buttonSize = PointF(32, 32);
        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        const float totalWidth = 5 * buttonSize.x + 4 * spacing;

        // calculate the starting x position for centering
        const float startPosX = (ImGui::GetWindowContentRegionMax().x - totalWidth) * 0.5f;
        ImGui::SetCursorPosX(startPosX);

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1, 1, 1, 0.3f));

        ImGui::PushID(0);
        if (ImGui::ImageButton(*m_IconPencil, buttonSize, 0))
        {
            m_CurrentTool = TileMapTool::Paint;
        }
        ImGui::SetItemTooltip("Paint Tool (B)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(1);
        if (ImGui::ImageButton(*m_IconEraser, buttonSize, 0))
        {
            m_CurrentTool = TileMapTool::Erase;
        }
        ImGui::SetItemTooltip("Erase Tool (E)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(2);
        if (ImGui::ImageButton(*m_IconPicker, buttonSize, 0))
        {
            m_CurrentTool = TileMapTool::Picker;
        }
        ImGui::SetItemTooltip("Picker Tool (I)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(3);
        if (ImGui::ImageButton(*m_IconFill, buttonSize, 0))
        {
            m_CurrentTool = TileMapTool::Fill;
        }
        ImGui::SetItemTooltip("Fill Tool (G)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(4);
        if (ImGui::ImageButton(*m_IconRect, buttonSize, 0))
        {
            m_CurrentTool = TileMapTool::Rect;
        }
        ImGui::SetItemTooltip("Box Fill Tool (U)");
        ImGui::PopID();

        ImGui::PopStyleColor(2);
    }

    void TilemapWindow::TileSelector(bool isWindowHovered)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.5);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.5f);

        ImGui::BeginChild("Tileset", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        float scrollX = 0.0f;
        float scrollY = 0.0f;

        int32_t mouseButton = 2;

        if (isWindowHovered && ImGui::IsMouseDragging(mouseButton, 0.0f))
        {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            scrollX -= delta.x;
            scrollY -= delta.y;
        }

        auto* currentTile = m_TilesetManager.GetSelectedTileset();
        ImVec2 tileSize = ImVec2(64, 64);
        Point tilesetSize = currentTile->GetSize();

        for (int32_t y = 0; y < tilesetSize.y; y++)
        {
            for (int32_t x = 0; x < tilesetSize.x; x++)
            {
                int32_t index = x + y * tilesetSize.x;
                Tile& tile = currentTile->GetTile(index);

                ImGui::PushID(&tile);
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0, 0, 0.5f, 0.3f));

                if (ImGui::ImageButton(tile.GetSprite(), tileSize))
                {
                    m_TileMap.SetSelectedTile(currentTile->GetID(), index);
                }

                ImGui::PopStyleColor(2);
                ImGui::PopID();
                ImGui::SameLine();
            }

            ImGui::NewLine();
        }

        ImGui::SetScrollX(ImGui::GetScrollX() + scrollX);
        ImGui::SetScrollY(ImGui::GetScrollY() + scrollY);

        ImGui::EndChild();
        ImGui::PopStyleVar(4);
    }

    void TilemapWindow::SelectLayer()
    {
        auto& layers = m_TileMap.GetLayerOrder();
        int selectedLayerIndex = 0;

        if (ImGui::BeginCombo("Layers", m_TileMap.GetCurrentLayer().c_str()))
        {
            for (int i = 0; i < layers.size(); ++i)
            {
                bool isSelected = (i == selectedLayerIndex);

                if (ImGui::Selectable(layers[i].c_str(), isSelected))
                {
                    selectedLayerIndex = i;

                    m_TileMap.SetCurrentLayer(layers[i]);
                }
            }

            ImGui::EndCombo();
        }
    }

    void TilemapWindow::SelectTileset()
    {
        if (ImGui::BeginTabBar("Tilesets"))
        {
            for (auto& [id, tileset] : m_TilesetManager.GetTilesets())
            {
                if (ImGui::BeginTabItem(tileset.GetName().c_str()))
                {
                    m_TilesetManager.SetSelectedTileset(&tileset);
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }
    }

    bool TilemapWindow::OnKeyPressed(const KeyPressedEvent& e)
    {
        bool control = Input::IsKeyPressed(KeyCode::Ctrl);

        switch (e.GetKeyCode())
        {
            case KeyCode::B:
                m_CurrentTool = TileMapTool::Paint;
                break;
            case KeyCode::E:
                m_CurrentTool = TileMapTool::Erase;
                break;
            case KeyCode::I:
                m_CurrentTool = TileMapTool::Picker;
                break;
            case KeyCode::G:
                m_CurrentTool = TileMapTool::Fill;
                break;
            case KeyCode::U:
                m_CurrentTool = TileMapTool::Rect;
                break;
            case KeyCode::S:
                if (control)
                {
                    //m_Serializer.SerialSizeTilemap(m_TileMape);
                    m_IsSaved = true;
                }
                break;
            default:
                break;
        }

        return false;
    }

    bool TilemapWindow::OnMouseButtonDown(const MouseButtonPressedEvent& e)
    {
        if (e.GetMouseButton() == MouseCode::Left)
        {
            m_MouseButtonHeld = true;
            m_PressedMousePosition = m_MousePosition;

            return true;
        }

        return false;
    }

    bool TilemapWindow::OnMouseButtonUp(const MouseButtonReleasedEvent& e)
    {
        if (e.GetMouseButton() == MouseCode::Left)
        {
            m_MouseButtonHeld = false;

            return true;
        }

        return false;
    }

} // Maize