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
        Tileset& tileset = m_Tilesets.back();
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
        sf::Vector2f windowSize = ImGui::GetContentRegionAvail();

        for (auto &tileset: m_Tilesets)
        {
            std::string text = tileset.GetName() + " ID: " + std::to_string(tileset.GetID());
            sf::Vector2f buttonPos = ImGui::GetCursorScreenPos();

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
        if (m_SelectedTileset == nullptr) return;

        int32_t id = m_SelectedTileset->GetID();
        std::string name = m_SelectedTileset->GetName();
        const uint32_t bufferSize = 128;
        char buffer[bufferSize];
        strncpy(buffer, name.c_str(), bufferSize);
		sf::Vector2i tileSize = m_SelectedTileset->GetTileSize();

        if (ImGui::InputInt("ID", &id, 0))
            m_SelectedTileset->SetID(std::max(id, 0));

        if (ImGui::InputText("Name", buffer, bufferSize))
            m_SelectedTileset->SetName(buffer);

        TextureSelector();

        if (ImGui::InputInt("Tile Size X", &tileSize.x, 1))
            m_SelectedTileset->SetTileSize(sf::Vector2i(std::max(tileSize.x, 1), tileSize.y));

        if (ImGui::InputInt("Tile Size Y", &tileSize.y, 1))
			m_SelectedTileset->SetTileSize(sf::Vector2i(tileSize.x, std::max(tileSize.y, 1)));
    }

    void TilesetWindowTab::TextureSelector()
    {
        if (m_SelectedTileset->GetTexture() != nullptr)
        {
            ImGui::Image(*m_SelectedTileset->GetTexture(), {64, 64});
            ImGui::SameLine();

            if (ImGui::Button("Select Texture"))
            {
                std::string file = FileDialog::OpenFile({ {"Tilesets (.png)", "*.png"} });

                if (!file.empty())
                {
                    m_SelectedTileset->SetTexture(file);
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

        const Texture* texture = m_SelectedTileset->GetTexture();
		sf::Vector2i tilesetSize = sf::Vector2i(texture->GetWidth() / m_SelectedTileset->GetTileSize().x, texture->GetHeight() / m_SelectedTileset->GetTileSize().y);

        static float scaleFactor = 4.0f;
        float scaledImageSizeX = (float)texture->GetWidth() * scaleFactor;
        float scaledImageSizeY = (float)texture->GetHeight() * scaleFactor;

        float scrollX = 0.0f;
        float scrollY = 0.0f;

        // handle scaling with mouse wheel
        ImGuiIO& io = ImGui::GetIO();
        if (io.MouseWheel != 0)
        {
            float scrollDelta = io.MouseWheel;
            scaleFactor += scrollDelta;
            scaleFactor = std::clamp(scaleFactor, 1.0f, 10.0f);
        }

        // handle panning with middle mouse button
        if (ImGui::IsMouseDragging(2, 0.0f))
        {
            ImVec2 delta = ImGui::GetIO().MouseDelta;
            scrollX -= delta.x;
            scrollY -= delta.y;
        }

        sf::Vector2f imagePos = ImGui::GetCursorScreenPos();
        ImGui::Image(*texture, { scaledImageSizeX, scaledImageSizeY });

        // loop through tiles and display them
        for (int32_t x = 0; x < tilesetSize.x; x++)
        {
            for (int32_t y = 0; y < tilesetSize.y; y++)
            {
                int32_t index = x + y * tilesetSize.x;

                ImGui::PushID(index);

                // scale the button size along with the image
                ImGui::SetNextItemAllowOverlap();
                sf::Vector2f buttonSize = sf::Vector2f(m_SelectedTileset->GetTileSize().x * scaleFactor, m_SelectedTileset->GetTileSize().y * scaleFactor);
                ImGui::SetCursorScreenPos({ imagePos.x + x * buttonSize.x, imagePos.y + y * buttonSize.y });

                // change appearance of tile depending on the tile state
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, m_SelectedTileset->HasTile(index) ? 0.0f : 0.65f));
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.35f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, m_SelectedTileset->HasTile(index) ? 0.45f : 0.9f));

                // tell the user more information about each tile
                if (ImGui::IsItemHovered())
                {
                    ImGui::SetTooltip("Atlas Position [%d, %d]", x, y);
                }

                // toggle tile to be a part of atlas
                if (ImGui::Button("##Select", buttonSize))
                {
					m_SelectedTileset->IncludeTile(x, y);
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