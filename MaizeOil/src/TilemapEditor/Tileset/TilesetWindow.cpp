#include "TilesetWindow.h"

namespace Maize {

    void TilesetWindow::Window()
    {
        ImGui::Begin("Tileset");

        float mainWindowWidth = ImGui::GetWindowContentRegionWidth();

        ImGui::Columns(3, "TilesetColumns", true);

        ImGui::BeginChild("Atlases", {0, mainWindowWidth / 2});
        SelectAtlas();
        ImGui::EndChild();

        if (ImGui::ImageButton(*m_IconAdd, {16, 16}))
        {
            AddAtlas();
        }
        ImGui::SameLine();
        if (ImGui::ImageButton(*m_IconRemove, {16, 16}))
        {
            if (m_SelectedAtlas != nullptr)
            {
                RemoveAtlas(m_SelectedAtlas->GetID());
            }
        }

        ImGui::NextColumn();

        ImGui::BeginChild("Current Atlas", {0, mainWindowWidth / 2});
        ShowCurrentAtlas();
        ImGui::EndChild();

        ImGui::NextColumn();

        ImGui::BeginChild("Show Atlas Tiles");
        HandleAtlasTiles();
        ImGui::EndChild();

        ImGui::Columns(2);

        ImGui::End();
    }

    TileAtlas& TilesetWindow::AddAtlas()
    {
        m_Atlases.emplace_back();
        auto& atlas = m_Atlases.back();
        atlas.SetID(CreateID()); // temp
        m_SelectedAtlas = &atlas;

        return atlas;
    }

    void TilesetWindow::RemoveAtlas(int32_t atlasID)
    {
        for (auto it = m_Atlases.rbegin(); it != m_Atlases.rend(); ++it)
        {
            if (it->GetID() == atlasID)
            {
                auto eraseIt = it.base() - 1;

                m_Atlases.erase(eraseIt);
                m_SelectedAtlas = m_Atlases.empty() ? nullptr : &m_Atlases.back();

                break;
            }
        }
    }

    void TilesetWindow::SelectAtlas()
    {
        auto windowSize = ImGui::GetContentRegionAvail();

        for (auto &atlas: m_Atlases)
        {
            if (atlas.GetTexture() != nullptr)
                ImGui::Image(*atlas.GetTexture(), {64, 64});

            ImGui::SameLine(0, 0.1f);

            std::string text = std::format("{} (ID: {})", atlas.GetName(), atlas.GetID());

            if (ImGui::Button(text.c_str(), {windowSize.x - 64, 64}))
            {
                m_SelectedAtlas = &atlas;
            }
        }
    }

    void TilesetWindow::ShowCurrentAtlas()
    {
        if (m_SelectedAtlas != nullptr)
        {
            int32_t id = m_SelectedAtlas->GetID();
            std::string name = m_SelectedAtlas->GetName();
            const uint32_t bufferSize = 128;
            char buffer[bufferSize];
            strncpy(buffer, name.c_str(), bufferSize);
            int32_t tileSizeX = m_SelectedAtlas->GetTileSizeX();
            int32_t tileSizeY = m_SelectedAtlas->GetTileSizeY();

            ImGui::Text("Atlas");
            if (ImGui::InputInt("ID", &id))
            {
                if (id < 0) id = 0;
                m_SelectedAtlas->SetID(id);
            }

            if (ImGui::InputText("Name", buffer, bufferSize))
            {
                m_SelectedAtlas->SetName(buffer);
            }

            TextureSelector();

            if (ImGui::InputInt("Tile Size X", &tileSizeX))
            {
                if (tileSizeX < 1) tileSizeX = 1;
                m_SelectedAtlas->SetTileSizeX(tileSizeX);
            }

            if (ImGui::InputInt("Tile Size Y", &tileSizeY))
            {
                if (tileSizeY < 1) tileSizeY = 1;
                m_SelectedAtlas->SetTileSizeY(tileSizeY);
            }
        }
    }

    void TilesetWindow::TextureSelector()
    {
        if (m_SelectedAtlas->GetTexture() != nullptr)
        {
            ImGui::Image(*m_SelectedAtlas->GetTexture(), {64, 64});
            ImGui::SameLine();

            if (ImGui::Button("Select Texture"))
            {
                auto file = pfd::open_file("Choose Tileset", "Assets", {"Tilesets (.png)", "*.png"}).result();

                if (!file.empty())
                {
                    m_SelectedAtlas->SetTexture(file[0]);
                    ImGui::OpenPopup("AutoTilesPopup");
                }
            }
        }

        SetAutomaticTiles();
    }

    void TilesetWindow::SetAutomaticTiles()
    {
        if (ImGui::BeginPopupModal("AutoTilesPopup"))
        {
            ImGui::Text("Automatically generate tiles that are not transparent?");

            if (ImGui::Button("Yes"))
            {
                m_SelectedAtlas->Clear();
                m_SelectedAtlas->AutoSetTiles(false);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                m_SelectedAtlas->Clear();
                m_SelectedAtlas->AutoSetTiles(true);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Ignore"))
            {
                m_SelectedAtlas->Clear();
                m_SelectedAtlas->InitEmptyTiles();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void TilesetWindow::HandleAtlasTiles()
    {
        if (m_SelectedAtlas == nullptr) return;

        const auto* texture = m_SelectedAtlas->GetTexture();
        int32_t tilesetSizeX = (int32_t)texture->GetWidth() / m_SelectedAtlas->GetTileSizeX();
        int32_t tilesetSizeY = (int32_t)texture->GetHeight() / m_SelectedAtlas->GetTileSizeY();
        float scaleFactor = 2.0f;
        float scaledImageSizeX = (float)texture->GetWidth() * scaleFactor;
        float scaledImageSizeY = (float)texture->GetHeight() * scaleFactor;

        auto imagePos = ImGui::GetCursorScreenPos();
        ImGui::Image(*texture, { scaledImageSizeX, scaledImageSizeY });

        for (int32_t x = 0; x < tilesetSizeX; x++)
        {
            for (int32_t y = 0; y < tilesetSizeY; y++)
            {
                int32_t index = x + y * tilesetSizeX;
                auto* tile = m_SelectedAtlas->GetTile(index);

                if (tile == nullptr) continue;

                ImGui::PushID(tile);

                // scale the button size along with the image
                ImGui::SetNextItemAllowOverlap();
                auto buttonSize = ImVec2(m_SelectedAtlas->GetTileSizeX() * scaleFactor, m_SelectedAtlas->GetTileSizeY() * scaleFactor);
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
    }

} // Maize