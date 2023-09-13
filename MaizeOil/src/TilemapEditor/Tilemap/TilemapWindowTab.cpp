#include "TilemapWindowTab.h"

namespace Maize {

    TilemapWindowTab::TilemapWindowTab(std::vector<Tileset> &tilesets) : m_Tilesets(tilesets)
    {
        m_IconPencil = Texture::Create("Resources/Icons/pencil.png");
        m_IconEraser = Texture::Create("Resources/Icons/eraser.png");
        m_IconPicker = Texture::Create("Resources/Icons/color-picker.png");
        m_IconFill = Texture::Create("Resources/Icons/fill.png");
        m_IconRect = Texture::Create("Resources/Icons/rect.png");
    }

    void TilemapWindowTab::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(std::bind(&TilemapWindowTab::OnKeyPressed, this, std::placeholders::_1));
    }

    void TilemapWindowTab::Window()
    {
        if (ImGui::BeginTabItem(("Tilemap")))
        {
            float mainWindowWidth = ImGui::GetWindowWidth();

            ButtonTools();

            ImGui::Columns(2, "TilesetColumns", true);

            ImGui::BeginChild("Tilesets", {0, mainWindowWidth / 2});
            SelectTileset();
            ImGui::EndChild();

            ImGui::NextColumn();

            ImGui::BeginChild("Select Tile");
            SelectTile();
            ImGui::EndChild();

            ImGui::Columns(1);

            ImGui::EndTabItem();
        }
    }

    void TilemapWindowTab::ButtonTools()
    {
        const auto buttonSize = ImVec2(16, 16);

        ImGui::PushID(0);
        if (ImGui::ImageButton(*m_IconPencil, buttonSize))
        {
            m_CurrentTool = Tools::Pencil;
        }
        ImGui::SetItemTooltip("Paint Tool (B)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(1);
        if (ImGui::ImageButton(*m_IconEraser, buttonSize))
        {
            m_CurrentTool = Tools::Erase;
        }
        ImGui::SetItemTooltip("Erase Tool (E)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(2);
        if (ImGui::ImageButton(*m_IconPicker, buttonSize))
        {
            m_CurrentTool = Tools::Picker;
        }
        ImGui::SetItemTooltip("Picker Tool (I)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(3);
        if (ImGui::ImageButton(*m_IconFill, buttonSize))
        {
            m_CurrentTool = Tools::Fill;
        }
        ImGui::SetItemTooltip("Fill Tool (G)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(4);
        if (ImGui::ImageButton(*m_IconRect, buttonSize))
        {
            m_CurrentTool = Tools::Rect;
        }
        ImGui::SetItemTooltip("Box Fill Tool (U)");
        ImGui::PopID();
    }

    void TilemapWindowTab::SelectTileset()
    {
        auto windowSize = ImGui::GetContentRegionAvail();

        for (auto& tileset: m_Tilesets)
        {
            std::string text = std::format("{} (ID: {})", tileset.GetName(), tileset.GetID());
            auto buttonPos = ImGui::GetCursorScreenPos();

            if (ImGui::Button(text.c_str(), {windowSize.x, 64}))
            {
                m_SelectedTileset = &tileset;
            }

            ImGui::SameLine(0, 0.1f);

            ImGui::SetNextItemAllowOverlap();
            ImGui::SetCursorScreenPos({ buttonPos.x, buttonPos.y });

            if (tileset.GetTexture() != nullptr)
            {
                ImGui::Image(*tileset.GetTexture(), {64, 64});
            }
        }
    }

    void TilemapWindowTab::SelectTile()
    {
        if (m_SelectedTileset == nullptr || !m_SelectedTileset->HasTexture()) return;

        ImGui::BeginChild("Tilemap", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        const auto* texture = m_SelectedTileset->GetTexture();
        int32_t tilesetSizeX = (int32_t)texture->GetWidth() / m_SelectedTileset->GetTileSizeX();
        int32_t tilesetSizeY = (int32_t)texture->GetHeight() / m_SelectedTileset->GetTileSizeY();
        static float scaleFactor = 4.0f;
        float scaledImageSizeX = (float)texture->GetWidth() * scaleFactor;
        float scaledImageSizeY = (float)texture->GetHeight() * scaleFactor;

        float scrollX = 0.0f;
        float scrollY = 0.0f;

        ImGuiIO& io = ImGui::GetIO();

        if (io.MouseWheel != 0)
        {
            float scrollDelta = io.MouseWheel;
            if (scrollDelta > 0.0f)
            {
                scaleFactor++;
            }
            else if (scrollDelta < 0.0f)
            {
                scaleFactor--;
            }
        }

        if (scaleFactor >= 10) scaleFactor = 10;
        if (scaleFactor <= 1) scaleFactor = 1;

        if (ImGui::IsMouseDragging(2, 0.0f))
        {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            scrollX -= delta.x;
            scrollY -= delta.y;
        }

        auto imagePos = ImGui::GetCursorScreenPos();
        ImGui::Image(*texture, { scaledImageSizeX, scaledImageSizeY });

        for (int32_t x = 0; x < tilesetSizeX; x++)
        {
            for (int32_t y = 0; y < tilesetSizeY; y++)
            {
                int32_t index = x + y * tilesetSizeX;
                auto* tile = m_SelectedTileset->GetTile(index);

                if (tile == nullptr) continue;

                ImGui::PushID(tile);

                // scale the button size along with the image
                ImGui::SetNextItemAllowOverlap();
                auto buttonSize = ImVec2(m_SelectedTileset->GetTileSizeX() * scaleFactor, m_SelectedTileset->GetTileSizeY() * scaleFactor);
                ImGui::SetCursorScreenPos({ imagePos.x + x * buttonSize.x, imagePos.y + y * buttonSize.y });

                // change appearance of tile depending on the tile state
                if (!tile->IsIncluded())
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.65f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.65f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.65f));

                    ImGui::Button("##Select", buttonSize);
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.35f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.45f));

                    if (ImGui::Button("##Select", buttonSize))
                    {
                        SetSelectedTile(tile->GetTilesetID(), tile->GetIndex());
                    }
                }

                ImGui::PopStyleColor(3);

                ImGui::PopID();
                ImGui::SameLine();
            }

            ImGui::NewLine();
        }

        ImGui::SetScrollX(ImGui::GetScrollX() + scrollX);
        ImGui::SetScrollY(ImGui::GetScrollY() + scrollY);

        ImGui::EndChild();
    }

    void TilemapWindowTab::SetSelectedTile(int32_t tilesetID, int32_t index)
    {
        m_SelectedTile.tilesetID = tilesetID;
        m_SelectedTile.index = index;
    }

    bool TilemapWindowTab::OnKeyPressed(const KeyPressedEvent &e)
    {
        bool control = Input::IsKeyPressed(KeyCode::Ctrl);

        if (e.GetKeyCode() == KeyCode::LeftBracket)
        {
            if (control)
            {
                m_CurrentRotation -= 90.0f;

                if (m_CurrentRotation < 0.0f) m_CurrentRotation = 270.0f;
            }
            else
            {
                m_FlipTileX = !m_FlipTileX;
            }

            return true;
        }

        if (e.GetKeyCode() == KeyCode::RightBracket)
        {
            if (control)
            {
                m_CurrentRotation += 90.0f;

                if (m_CurrentRotation > 270.0f) m_CurrentRotation = 0.0f;
            }
            else
            {
                m_FlipTileY = !m_FlipTileY;
            }

            return true;
        }

        switch (e.GetKeyCode())
        {
            case KeyCode::B:
                m_CurrentTool = Tools::Pencil;
                break;
            case KeyCode::E:
                m_CurrentTool = Tools::Erase;
                break;
            case KeyCode::I:
                m_CurrentTool = Tools::Picker;
                break;
            case KeyCode::G:
                m_CurrentTool = Tools::Fill;
                break;
            case KeyCode::U:
                m_CurrentTool = Tools::Rect;
                break;
            default:
                break;
        }

        return false;
    }

} // Maize