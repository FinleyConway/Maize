#include "TilemapWindowTab.h"

namespace Maize {

    TilemapWindowTab::TilemapWindowTab()
    {
        m_IconPencil = Texture::Create("Resources/Icons/pencil.png");
        m_IconEraser = Texture::Create("Resources/Icons/eraser.png");
        m_IconPicker = Texture::Create("Resources/Icons/color-picker.png");
		m_IconAdd = Texture::Create("Resources/Icons/plus.png");
    }

    void TilemapWindowTab::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(std::bind(&TilemapWindowTab::OnKeyPressed, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&TilemapWindowTab::OnMousePressed, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&TilemapWindowTab::OnMouseReleased, this, std::placeholders::_1));
    }

	void TilemapWindowTab::Window(std::unordered_map<int32_t, Tileset>& tilesets, std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent)
	{
		if (ImGui::BeginTabItem(("Tilemap")))
		{
			OnUpdate(editorGrid, tilemapComponent);

			float mainWindowWidth = ImGui::GetWindowWidth();

			ButtonTools();
			ImGui::SameLine();
			TilemapLayers(editorGrid, tilemapComponent);

			ImGui::Columns(2, "TilesetColumns", true);

			ImGui::BeginChild("Tilesets", { 0, mainWindowWidth / 2 });
			SelectTileset(tilesets);
			ImGui::EndChild();

			ImGui::NextColumn();

			ImGui::BeginChild("Select Tile");
			SelectTile();
			ImGui::EndChild();

			ImGui::Columns(1);

			ImGui::EndTabItem();
		}
	}

    void TilemapWindowTab::OnUpdate(std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent)
    {
        if (tilemapComponent->tilemapLayers.empty() || editorGrid.empty()) return;

        auto& tilemap = tilemapComponent->tilemapLayers[m_SelectedTilemapLayer];
		auto& editorMap = editorGrid[m_SelectedTilemapLayer];

        sf::Vector2f mousePosition = Camera::ScreenToWorld(Input::GetMousePosition());
        sf::Vector2i gridPosition = CartesianGrid<TilemapEditorTile>::ConvertScreenToGrid(mousePosition, { tilemapComponent->tileSizeX, tilemapComponent->tileSizeY });
		sf::Vector2i size = sf::Vector2i(tilemapComponent->tileSizeX, tilemapComponent->tileSizeY);

        if (m_MouseLeftHeld)
        {
            if (m_CurrentTool == TilemapTools::Pencil)
            {
				if (m_SelectedTile.IsValid())
				{
					editorMap.grid.InsertTile(gridPosition, true, m_SelectedTile.tilesetID, m_SelectedTile.tileIndex, m_SelectedTile.texCoords, m_FlipTileX, m_FlipTileY, m_CurrentRotation);
					tilemap.InsertTile(gridPosition, Renderer::CreateQuad((sf::Vector2f)gridPosition, (sf::Vector2f)size, (sf::Vector2f)m_SelectedTile.texCoords), true);
				}
            }
            else if (m_CurrentTool == TilemapTools::Erase)
            {
				editorMap.grid.RemoveTile(gridPosition);
				tilemap.RemoveTile(gridPosition);
            }
            else if (m_CurrentTool == TilemapTools::Picker)
            {
				const TilemapEditorTile* tile = editorMap.grid.GetTile(gridPosition);

				if (tile == nullptr) return;

				m_SelectedTile = *tile;
				m_FlipTileX = tile->flipX;
				m_FlipTileY = tile->flipY;
				m_CurrentRotation = tile->rotation;

				m_CurrentTool = TilemapTools::Pencil;
            }
        }
    }

    void TilemapWindowTab::ButtonTools()
    {
        const sf::Vector2f buttonSize = sf::Vector2f(16, 16);

        ImGui::PushID(0);
        if (ImGui::ImageButton(*m_IconPencil, buttonSize))
        {
            m_CurrentTool = TilemapTools::Pencil;
        }
        ImGui::SetItemTooltip("Paint Tool (B)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(1);
        if (ImGui::ImageButton(*m_IconEraser, buttonSize))
        {
            m_CurrentTool = TilemapTools::Erase;
        }
        ImGui::SetItemTooltip("Erase Tool (E)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(2);
        if (ImGui::ImageButton(*m_IconPicker, buttonSize))
        {
            m_CurrentTool = TilemapTools::Picker;
        }
        ImGui::SetItemTooltip("Picker Tool (I)");
        ImGui::PopID();
    }

    void TilemapWindowTab::TilemapLayers(std::vector<TilemapEditorLayer>& editorGrid, TilemapComponent* tilemapComponent)
    {
		std::string previewValue;

		ImGui::BeginDisabled(editorGrid.empty() || tilemapComponent->tilemapLayers.empty());

		// set preview value depending on the layer state
        if (editorGrid.empty() || tilemapComponent->tilemapLayers.empty())
        {
            previewValue = "No Layers";
        }
        else
        {
            previewValue = editorGrid[m_SelectedTilemapLayer].layerName;
        }

		// show all possible layers inside drop down
        if (ImGui::BeginCombo("##Layers", previewValue.c_str()))
        {
            for (uint32_t i = 0; i < editorGrid.size(); i++)
            {
                bool isSelected = (i == m_SelectedTilemapLayer);
                std::string tilemapLayer = editorGrid[i].layerName;

                if (ImGui::Selectable(tilemapLayer.c_str(), isSelected))
                {
                    m_SelectedTilemapLayer = i;
                }
            }
			
            ImGui::EndCombo();
        }

        ImGui::EndDisabled();

		ImGui::SameLine();

		if (ImGui::ImageButton(*m_IconAdd, sf::Vector2f(16, 16)))
		{
			ImGui::OpenPopup("CreateNewLayerPopup");
		}

		CreateTilemapLayer(editorGrid, tilemapComponent);
    }

	void TilemapWindowTab::CreateTilemapLayer(std::vector<TilemapEditorLayer>& editorGrid, Maize::TilemapComponent* tilemapComponent)
	{
		static std::string buffer;

		if (ImGui::BeginPopupModal("CreateNewLayerPopup"))
		{
			const uint8_t bufferSize = 255;
			char inputBuffer[bufferSize];
			strncpy(inputBuffer, buffer.c_str(), bufferSize);

			if (ImGui::InputText("New Layer Name", inputBuffer, bufferSize))
			{
				buffer = inputBuffer;
			}

			if (ImGui::Button("Create Layer") && !buffer.empty())
			{
				editorGrid.emplace_back(buffer);
				tilemapComponent->tilemapLayers.emplace_back();

				ImGui::CloseCurrentPopup();
				buffer.clear();
			}
			ImGui::SameLine();
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				buffer.clear();
			}

			ImGui::EndPopup();
		}
	}

	void TilemapWindowTab::SelectTileset(std::unordered_map<int32_t, Tileset>& tilesets)
    {
        sf::Vector2f windowSize = ImGui::GetContentRegionAvail();

		// list all available tilesets to allow selection
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
                ImGui::Image(*tileset.GetTexture(), { 64, 64 });
            }
        }
    }

    void TilemapWindowTab::SelectTile()
    {
        if (m_SelectedTileset == nullptr || !m_SelectedTileset->HasTexture()) return;

        ImGui::BeginChild("Tilemap", sf::Vector2f(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        const auto texture = m_SelectedTileset->GetTexture();
        sf::Vector2u tilesetSize = sf::Vector2u(texture->GetWidth() / m_SelectedTileset->GetTileSize().x, texture->GetHeight() / m_SelectedTileset->GetTileSize().y);
        float scaledImageSizeX = (float)texture->GetWidth() * m_TilesetZoomFactor;
        float scaledImageSizeY = (float)texture->GetHeight() * m_TilesetZoomFactor;

        float scrollX = 0.0f;
        float scrollY = 0.0f;

		// tileset zooming
        ImGuiIO& io = ImGui::GetIO();
        if (io.MouseWheel != 0)
        {
            float scrollDelta = io.MouseWheel;
            if (scrollDelta > 0.0f)
            {
				m_TilesetZoomFactor++;
            }
            else if (scrollDelta < 0.0f)
            {
				m_TilesetZoomFactor--;
            }
        }
        if (m_TilesetZoomFactor >= 10) m_TilesetZoomFactor = 10;
        if (m_TilesetZoomFactor <= 1) m_TilesetZoomFactor = 1;

		// tileset dragging
        if (ImGui::IsMouseDragging(2, 0.0f))
        {
            sf::Vector2f delta = ImGui::GetIO().MouseDelta;
            scrollX -= delta.x;
            scrollY -= delta.y;
        }

        sf::Vector2f imagePos = ImGui::GetCursorScreenPos();
        ImGui::Image(*texture, { scaledImageSizeX, scaledImageSizeY });

		// create buttons for where each tile would be in the tileset
        for (int32_t x = 0; x < tilesetSize.x; x++)
        {
            for (int32_t y = 0; y < tilesetSize.y; y++)
            {
                int32_t index = x + y * tilesetSize.x;
                Tile* tile = m_SelectedTileset->GetTile(index);

                ImGui::PushID(tile);

                // scale the button size along with the image
                ImGui::SetNextItemAllowOverlap();
                sf::Vector2f buttonSize = sf::Vector2f(m_SelectedTileset->GetTileSize().x * m_TilesetZoomFactor, m_SelectedTileset->GetTileSize().y * m_TilesetZoomFactor);
                ImGui::SetCursorScreenPos({ imagePos.x + x * buttonSize.x, imagePos.y + y * buttonSize.y });

                // change appearance of tile depending on the tile state
                if (!m_SelectedTileset->HasTile(index))
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
						m_SelectedTile.tileIndex = index;
						m_SelectedTile.tilesetID = tile->tilesetID;
                        m_SelectedTile.texCoords = tile->texCoords;
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
                m_CurrentTool = TilemapTools::Pencil;
                break;
            case KeyCode::E:
                m_CurrentTool = TilemapTools::Erase;
                break;
            case KeyCode::I:
                m_CurrentTool = TilemapTools::Picker;
                break;
            default:
                break;
        }

        return false;
    }

    bool TilemapWindowTab::OnMousePressed(const MouseButtonPressedEvent &e)
    {
        if (e.GetMouseButton() == MouseCode::Left)
        {
            m_MouseLeftHeld = true;

            return true;
        }

        return false;
    }

    bool TilemapWindowTab::OnMouseReleased(const MouseButtonReleasedEvent &e)
    {
        if (e.GetMouseButton() == MouseCode::Left)
        {
            m_MouseLeftHeld = false;

            return true;
        }

        return false;
    }

} // Maize