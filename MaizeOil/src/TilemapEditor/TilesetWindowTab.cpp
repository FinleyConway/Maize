#include "TilesetWindowTab.h"

namespace Maize {

    TilesetWindowTab::TilesetWindowTab()
    {
        m_IconAdd = Texture::Create("Resources/Icons/plus.png");
        m_IconRemove = Texture::Create("Resources/Icons/trash-can.png");
    }

    void TilesetWindowTab::Window()
    {
        if (ImGui::BeginTabItem("Tileset"))
        {
            float mainWindowWidth = ImGui::GetWindowWidth();

            if (ImGui::ImageButton(*m_IconAdd, { 16, 16 }))
            {
                AddTileset();
            }
            ImGui::SameLine();
            if (ImGui::ImageButton(*m_IconRemove, { 16, 16 }))
            {
                if (m_SelectedTileset != nullptr)
                {
                    RemoveTileset(m_SelectedTileset->GetID());
                }
            }

            ImGui::Columns(3, "TilesetColumns", true);

            ImGui::BeginChild("Tilesets", { 0, mainWindowWidth / 2 });
            SelectTileset();
            ImGui::EndChild();

            ImGui::NextColumn();

            ImGui::BeginChild("Current Tileset", { 0, mainWindowWidth / 2 });
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
        int32_t id = CreateID();

        m_TilemapComponent->tilesets.try_emplace(id);

        Tileset& tileset = m_TilemapComponent->tilesets[id];
        tileset.SetID(id); // temp
        m_SelectedTileset = &tileset;

        return tileset;
    }

    void TilesetWindowTab::RemoveTileset(int32_t tilesetID)
    {
		// if selected tileset is within tilesets
        if (m_TilemapComponent->tilesets.contains(tilesetID))
        {
			m_TilemapComponent->tilesets.erase(tilesetID);
			m_SelectedTileset = nullptr;

            UpdateMap();

			// auto select a new tileset if there is one there
			if (!m_TilemapComponent->tilesets.empty())
			{
				m_SelectedTileset = &m_TilemapComponent->tilesets.begin()->second;
			}
        }
    }

    void TilesetWindowTab::SelectTileset()
    {
        sf::Vector2f windowSize = ImGui::GetContentRegionAvail();

		// list though all the tilesets to be selected
        for (auto& [id, tileset]: m_TilemapComponent->tilesets)
        {
            std::string text = tileset.GetName() + " ID: " + std::to_string(tileset.GetID());
            sf::Vector2f buttonPos = ImGui::GetCursorScreenPos();

            if (ImGui::Button(text.c_str(), { windowSize.x, 64 }))
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
        const uint8_t bufferSize = 255;
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
            ImGui::Image(*m_SelectedTileset->GetTexture(), { 64, 64 });
            ImGui::SameLine();

            if (ImGui::Button("Select Texture"))
            {
                std::string file = FileDialog::OpenFile({ { "Tilesets (.png)", "*.png" } });

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

				UpdateMap();

                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->AutoSetTiles(true);

				UpdateMap();

                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Ignore"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->InitEmptyTiles();

				UpdateMap();

                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void TilesetWindowTab::HandleTilesetTiles()
    {
        if (m_SelectedTileset == nullptr || !m_SelectedTileset->HasTexture()) return;

        ImGui::BeginChild("Tileset", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        const auto texture = m_SelectedTileset->GetTexture();
        sf::Vector2u tilesetSize = sf::Vector2u(texture->GetWidth() / m_SelectedTileset->GetTileSize().x, texture->GetHeight() / m_SelectedTileset->GetTileSize().y);
        float scaledImageSizeX = (float)texture->GetWidth() * m_TilesetZoomFactor;
        float scaledImageSizeY = (float)texture->GetHeight() * m_TilesetZoomFactor;

        float scrollX = 0.0f;
        float scrollY = 0.0f;

        // handle scaling with mouse wheel
        ImGuiIO& io = ImGui::GetIO();
        if (io.MouseWheel != 0)
        {
            float scrollDelta = io.MouseWheel;
			m_TilesetZoomFactor += scrollDelta;
			m_TilesetZoomFactor = std::clamp(m_TilesetZoomFactor, 1.0f, 10.0f);
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
                sf::Vector2f buttonSize = sf::Vector2f(m_SelectedTileset->GetTileSize().x * m_TilesetZoomFactor, m_SelectedTileset->GetTileSize().y * m_TilesetZoomFactor);
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