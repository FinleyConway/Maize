#include "TilemapWindowTab.h"

namespace Maize {

    TilemapWindowTab::TilemapWindowTab(std::vector<Tileset>& tilesets, std::vector<TilemapLayer>& tilemapLayers, int32_t& cellSizeX, int32_t& cellSizeY) :
            m_Tilesets(tilesets),
            m_TilemapLayers(tilemapLayers),
            m_CellSizeX(cellSizeX),
            m_CellSizeY(cellSizeY)
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
        dispatcher.Dispatch<MouseButtonPressedEvent>(std::bind(&TilemapWindowTab::OnMousePressed, this, std::placeholders::_1));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(std::bind(&TilemapWindowTab::OnMouseReleased, this, std::placeholders::_1));
    }

	void TilemapWindowTab::OnUpdate(float deltaTime)
	{
		if (m_TilemapLayers.empty()) return;

		TilemapLayer& currentLayer = m_TilemapLayers[m_SelectedTilemapLayer];
        sf::Vector2f mousePosition = Camera::ScreenToWorld(Input::GetMousePosition());
        sf::Vector2i gridPosition = CartesianGrid::ConvertScreenToGrid(mousePosition, m_CellSizeX, m_CellSizeY);

		if (m_MouseLeftHeld)
		{
			if (m_CurrentTool == TilemapTools::Pencil)
			{
				currentLayer.PlaceTile(m_SelectedTile, gridPosition, m_FlipTileX, m_FlipTileY, m_CurrentRotation);
			}
			else if (m_CurrentTool == TilemapTools::Erase)
			{
				currentLayer.RemoveTile(gridPosition);
			}
			else if (m_CurrentTool == TilemapTools::Picker)
			{
				currentLayer.GetTileInfo(gridPosition, m_SelectedTile, m_FlipTileX, m_FlipTileY, m_CurrentRotation);
			}
			else if (m_CurrentTool == TilemapTools::Fill)
			{
				currentLayer.FillTiles(gridPosition, m_SelectedTile, currentLayer.GetTile(gridPosition));
			}
		}
	}

    void TilemapWindowTab::Window()
    {
        if (ImGui::BeginTabItem(("Tilemap")))
        {
            float mainWindowWidth = ImGui::GetWindowWidth();

            ButtonTools();
            ImGui::SameLine();
            TilemapLayers();

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

        ImGui::SameLine();

        ImGui::PushID(3);
        if (ImGui::ImageButton(*m_IconFill, buttonSize))
        {
            m_CurrentTool = TilemapTools::Fill;
        }
        ImGui::SetItemTooltip("Fill Tool (G)");
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(4);
        if (ImGui::ImageButton(*m_IconRect, buttonSize))
        {
            m_CurrentTool = TilemapTools::Rect;
        }
        ImGui::SetItemTooltip("Box Fill Tool (U)");
        ImGui::PopID();
    }

    void TilemapWindowTab::TilemapLayers()
    {
		ImGui::BeginDisabled(m_TilemapLayers.empty());
		std::string previewValue;

		if (m_TilemapLayers.empty())
		{
			previewValue = "No Layers";
		}
		else
		{
			previewValue = m_TilemapLayers[m_SelectedTilemapLayer].GetName();
		}

        if (ImGui::BeginCombo("##Layers", previewValue.c_str()))
        {
            for (uint32_t i = 0; i < m_TilemapLayers.size(); i++)
            {
                bool isSelected = (i == m_SelectedTilemapLayer);
                std::string tilemapLayer = m_TilemapLayers[i].GetName();

                if (ImGui::Selectable(tilemapLayer.c_str(), isSelected))
                {
                    m_SelectedTilemapLayer = i;
                }
            }

            ImGui::EndCombo();
        }

		ImGui::EndDisabled();
    }

    void TilemapWindowTab::SelectTileset()
    {
        sf::Vector2f windowSize = ImGui::GetContentRegionAvail();

        for (auto& tileset: m_Tilesets)
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

    void TilemapWindowTab::SelectTile()
    {
        if (m_SelectedTileset == nullptr || !m_SelectedTileset->HasTexture()) return;

        ImGui::BeginChild("Tilemap", sf::Vector2f(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

        const Texture* texture = m_SelectedTileset->GetTexture();
        int32_t tilesetSizeX = texture->GetWidth() / m_SelectedTileset->GetTileSizeX();
        int32_t tilesetSizeY = texture->GetHeight() / m_SelectedTileset->GetTileSizeY();
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
            sf::Vector2f delta = ImGui::GetIO().MouseDelta;
            scrollX -= delta.x;
            scrollY -= delta.y;
        }

        sf::Vector2f imagePos = ImGui::GetCursorScreenPos();
        ImGui::Image(*texture, { scaledImageSizeX, scaledImageSizeY });

        for (int32_t x = 0; x < tilesetSizeX; x++)
        {
            for (int32_t y = 0; y < tilesetSizeY; y++)
            {
                int32_t index = x + y * tilesetSizeX;
                Tile* tile = m_SelectedTileset->GetTile(index);

                if (tile == nullptr) continue;

                ImGui::PushID(tile);

                // scale the button size along with the image
                ImGui::SetNextItemAllowOverlap();
                sf::Vector2f buttonSize = sf::Vector2f(m_SelectedTileset->GetTileSizeX() * scaleFactor, m_SelectedTileset->GetTileSizeY() * scaleFactor);
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
                m_CurrentTool = TilemapTools::Pencil;
                break;
            case KeyCode::E:
                m_CurrentTool = TilemapTools::Erase;
                break;
            case KeyCode::I:
                m_CurrentTool = TilemapTools::Picker;
                break;
            case KeyCode::G:
                m_CurrentTool = TilemapTools::Fill;
                break;
            case KeyCode::U:
                m_CurrentTool = TilemapTools::Rect;
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