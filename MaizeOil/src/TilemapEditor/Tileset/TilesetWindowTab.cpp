#include "TilesetWindowTab.h"

namespace Maize {

    TilesetWindowTab::TilesetWindowTab(std::vector<Tileset>& tilesets) : m_Tilesets(tilesets)
    {
        m_IconAdd = Texture::Create("Resources/Icons/plus.png");
        m_IconRemove = Texture::Create("Resources/Icons/trash-can.png");
    }

    void TilesetWindowTab::Window()
    {
        if (ImGui::BeginTabItem("Tileset"))
        {
            float mainWindowWidth = ImGui::GetWindowWidth();

            if (ImGui::ImageButton(*m_IconAdd, {16, 16}))
            {
                AddTileset();
            }
            ImGui::SameLine();
            if (ImGui::ImageButton(*m_IconRemove, {16, 16}))
            {
                if (m_SelectedTileset != nullptr)
                {
                    RemoveTileset(m_SelectedTileset->GetID());
                }
            }

            ImGui::Columns(3, "TilesetColumns", true);

            ImGui::BeginChild("Tilesets", {0, mainWindowWidth / 2});
            SelectTileset();
            ImGui::EndChild();

            ImGui::NextColumn();

            ImGui::BeginChild("Current Tileset", {0, mainWindowWidth / 2});
            ShowCurrentTileset();
            ImGui::EndChild();

            ImGui::NextColumn();

            ImGui::BeginChild("Show Tileset Tiles");
            HandleTilesetTiles();
            ImGui::EndChild();

            ImGui::Columns(2);

            ImGui::EndTabItem();
        }
    }

    Tileset& TilesetWindowTab::AddTileset()
    {
        m_Tilesets.emplace_back();
        auto& tileset = m_Tilesets.back();
        tileset.SetID(CreateID()); // temp
        m_SelectedTileset = &tileset;

        return tileset;
    }

    void TilesetWindowTab::RemoveTileset(int32_t tilesetID)
    {
        for (auto it = m_Tilesets.rbegin(); it != m_Tilesets.rend(); ++it)
        {
            if (it->GetID() == tilesetID)
            {
                auto eraseIt = it.base() - 1;

                m_Tilesets.erase(eraseIt);
                m_SelectedTileset = m_Tilesets.empty() ? nullptr : &m_Tilesets.back();

                break;
            }
        }
    }

    void TilesetWindowTab::SelectTileset()
    {
        auto windowSize = ImGui::GetContentRegionAvail();

        for (auto &tileset: m_Tilesets)
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

    void TilesetWindowTab::ShowCurrentTileset()
    {
        if (m_SelectedTileset != nullptr)
        {
            int32_t id = m_SelectedTileset->GetID();
            std::string name = m_SelectedTileset->GetName();
            const uint32_t bufferSize = 128;
            char buffer[bufferSize];
            strncpy(buffer, name.c_str(), bufferSize);
            int32_t tileSizeX = m_SelectedTileset->GetTileSizeX();
            int32_t tileSizeY = m_SelectedTileset->GetTileSizeY();

            if (ImGui::InputInt("ID", &id))
            {
                if (id < 0) id = 0;
                m_SelectedTileset->SetID(id);
            }

            if (ImGui::InputText("Name", buffer, bufferSize))
            {
                m_SelectedTileset->SetName(buffer);
            }

            TextureSelector();

            if (ImGui::InputInt("Tile Size X", &tileSizeX))
            {
                if (tileSizeX < 1) tileSizeX = 1;
                m_SelectedTileset->SetTileSizeX(tileSizeX);
            }

            if (ImGui::InputInt("Tile Size Y", &tileSizeY))
            {
                if (tileSizeY < 1) tileSizeY = 1;
                m_SelectedTileset->SetTileSizeY(tileSizeY);
            }
        }
    }

    void TilesetWindowTab::TextureSelector()
    {
        if (m_SelectedTileset->GetTexture() != nullptr)
        {
            ImGui::Image(*m_SelectedTileset->GetTexture(), {64, 64});
            ImGui::SameLine();

            if (ImGui::Button("Select Texture"))
            {
                auto file = pfd::open_file("Choose Tileset", "Assets", {"Tilesets (.png)", "*.png"}).result();

                if (!file.empty())
                {
                    m_SelectedTileset->SetTexture(file[0]);
                    ImGui::OpenPopup("AutoTilesPopup");
                }
            }
        }

        SetAutomaticTiles();
    }

    void TilesetWindowTab::SetAutomaticTiles()
    {
        if (ImGui::BeginPopupModal("AutoTilesPopup"))
        {
            ImGui::Text("Automatically generate tiles that are not transparent?");

            if (ImGui::Button("Yes"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->AutoSetTiles(false);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->AutoSetTiles(true);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Ignore"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->InitEmptyTiles();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void TilesetWindowTab::HandleTilesetTiles()
    {
        if (m_SelectedTileset == nullptr || !m_SelectedTileset->HasTexture()) return;

        ImGui::BeginChild("Tileset", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

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
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.35f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.9f));
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.35f));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.45f));
                }

                // tell user more information about each tile
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Atlas Position [%u, %u]", x, y);
                }

                // toggle tile to be a part of atlas
                if (ImGui::Button("##Select", buttonSize))
                {
                    tile->IsIncluded(!tile->IsIncluded());
                    tile->SetIndex(index);
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

} // Maize