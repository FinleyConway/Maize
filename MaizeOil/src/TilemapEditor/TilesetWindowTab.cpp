#include "TilesetWindowTab.h"

namespace Maize {

    TilesetWindowTab::TilesetWindowTab()
    {
        m_IconAdd = Texture::Create("Resources/Icons/plus.png");
        m_IconRemove = Texture::Create("Resources/Icons/trash-can.png");
    }

    void TilesetWindowTab::Window(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<CartesianGrid<TilemapEditorTile>>& editorGrid, TilemapComponent* tilemapComponent)
    {
        if (ImGui::BeginTabItem("Tileset"))
        {
            float mainWindowWidth = ImGui::GetWindowWidth();

            if (ImGui::ImageButton(*m_IconAdd, { 16, 16 }))
            {
                AddTileset(tilesets);
            }
            ImGui::SameLine();
            if (ImGui::ImageButton(*m_IconRemove, { 16, 16 }))
            {
                if (m_SelectedTileset != nullptr)
                {
                    RemoveTileset(m_SelectedTileset->GetID(), tilesets, editorGrid, tilemapComponent);
                }
            }

            ImGui::Columns(3, "TilesetColumns", true);

            ImGui::BeginChild("Tilesets", { 0, mainWindowWidth / 2 });
            SelectTileset(tilesets);
            ImGui::EndChild();

            ImGui::NextColumn();

            ImGui::BeginChild("Current Tileset", { 0, mainWindowWidth / 2 });
            ShowCurrentTileset(tilesets, editorGrid, tilemapComponent);
            ImGui::EndChild();

            ImGui::NextColumn();

            ImGui::BeginChild("Show Tileset Tiles");
            HandleTilesetTiles();
            ImGui::EndChild();

            ImGui::Columns(2);

            ImGui::EndTabItem();
        }
    }

    Tileset& TilesetWindowTab::AddTileset(std::unordered_map<int32_t, Tileset>& tilesets)
    {
        int32_t id = CreateID();

        tilesets.try_emplace(id);

        Tileset& tileset = tilesets[id];
        tileset.SetID(id); // temp
        m_SelectedTileset = &tileset;

        return tileset;
    }

    void TilesetWindowTab::RemoveTileset(int32_t tilesetID, std::unordered_map<int32_t, Tileset>& tilesets, std::vector<CartesianGrid<TilemapEditorTile>>& editorGrid, TilemapComponent* tilemapComponent)
    {
		// if selected tileset is within tilesets
        if (tilesets.contains(tilesetID))
        {
			tilesets.erase(tilesetID);
			m_SelectedTileset = nullptr;

            UpdateMap(tilesets, editorGrid, tilemapComponent);

			// auto select a new tileset if there is one there
			if (!tilesets.empty())
			{
				m_SelectedTileset = &tilesets.begin()->second;
			}
        }
    }

    void TilesetWindowTab::SelectTileset(std::unordered_map<int32_t, Tileset>& tilesets)
    {
        sf::Vector2f windowSize = ImGui::GetContentRegionAvail();

		// list though all the tilesets to be selected
        for (auto& [id, tileset]: tilesets)
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

    void TilesetWindowTab::ShowCurrentTileset(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<CartesianGrid<TilemapEditorTile>>& editorGrid, TilemapComponent* tilemapComponent)
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

        TextureSelector(tilesets, editorGrid, tilemapComponent);

        if (ImGui::InputInt("Tile Size X", &tileSize.x, 1))
            m_SelectedTileset->SetTileSize(sf::Vector2i(std::max(tileSize.x, 1), tileSize.y));

        if (ImGui::InputInt("Tile Size Y", &tileSize.y, 1))
            m_SelectedTileset->SetTileSize(sf::Vector2i(tileSize.x, std::max(tileSize.y, 1)));
    }

    void TilesetWindowTab::TextureSelector(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<CartesianGrid<TilemapEditorTile>>& editorGrid, TilemapComponent* tilemapComponent)
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

        SetAutomaticTiles(tilesets, editorGrid, tilemapComponent);
    }

    void TilesetWindowTab::SetAutomaticTiles(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<CartesianGrid<TilemapEditorTile>>& editorGrid, TilemapComponent* tilemapComponent)
    {
        if (ImGui::BeginPopupModal("AutoTilesPopup"))
        {
            ImGui::Text("Automatically generate tiles that are not transparent?");

            if (ImGui::Button("Yes"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->AutoSetTiles(false);

				UpdateMap(tilesets, editorGrid, tilemapComponent);

                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->AutoSetTiles(true);

				UpdateMap(tilesets, editorGrid, tilemapComponent);

                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Ignore"))
            {
                m_SelectedTileset->Clear();
                m_SelectedTileset->InitEmptyTiles();

				UpdateMap(tilesets, editorGrid, tilemapComponent);

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

	void TilesetWindowTab::UpdateMap(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<CartesianGrid<TilemapEditorTile>>& editorGrid, TilemapComponent* tilemapComponent)
	{
		PackTileset(tilesets, tilemapComponent);

		for (uint32_t i = 0; i < editorGrid.size(); i++)
		{
			auto& editorMap = editorGrid[i];
			auto& tilemap = tilemapComponent->tilemapLayers[i];

			sf::Vector2i halfSize = editorMap.GridSize() / 2;

			for (int32_t y = -halfSize.y; y < halfSize.y; y++)
			{
				for (int32_t x = -halfSize.x; x < halfSize.x; x++)
				{
					const TilemapEditorTile* tile = editorMap.GetTile(sf::Vector2i(x, y));

					if (tile == nullptr) continue;

					Tile* tilesetTile = Tileset::FindTileByTilesetID(tilesets, tile->tilesetID, tile->tileIndex);

					if (tilesetTile == nullptr) continue;

					TilemapEditorTile newTile = *tile;
					newTile.texCoords = tilesetTile->texCoords;

					editorMap.InsertTile(sf::Vector2i(x, y), true, newTile.tilesetID, newTile.tileIndex, newTile.texCoords, newTile.flipX, newTile.flipY, newTile.rotation);
					tilemap.InsertTile(sf::Vector2i(x, y), Renderer::CreateQuad(sf::Vector2f(x, y), 0, (sf::Vector2f)tilesetTile->tileSize, (sf::Vector2f)tile->texCoords), true);
				}
			}
		}
	}

    void TilesetWindowTab::PackTileset(std::unordered_map<int32_t, Tileset>& tilesets, TilemapComponent* tilemapComponent)
    {
        std::vector<Maize::TextureInfo> textureInfo;

        // add textures to temp array
        textureInfo.reserve(tilesets.size());
        for (auto& [tilesetID, tileset] : tilesets)
        {
            textureInfo.emplace_back(*tileset.GetTexture(), tilesetID);
        }

        // pack textures into one big texture
        auto results = Maize::TexturePacker::Pack(textureInfo, sf::Vector2u(1024, 1024));

		tilemapComponent->tilemapTexture = std::make_shared<sf::Texture>(results.packedTexture);

        // offset all the tiles in the tileset of where they are in the packed texture
        for (auto& [tilesetID, tileset] : tilesets)
        {
            for (auto& [id, rect] : results.idToTextureRect)
            {
                if (id == tilesetID)
                {
                    for (auto& [tileID, tile] : tileset.GetTiles())
                    {
                        sf::Vector2i texturePosition = rect.getPosition();

                        tile.texCoords.x = tile.originalTexCoords.x + (texturePosition.x / tile.tileSize.x);
                        tile.texCoords.y = tile.originalTexCoords.y + (texturePosition.y / tile.tileSize.y);
                    }
                }
            }
        }
    }

} // Maize