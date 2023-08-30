#include "Tilemap.h"

namespace Maize {

    void Tilemap::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(std::bind(&Tilemap::OnKeyPressed, this, std::placeholders::_1));
    }

    void Tilemap::SetCurrentLayer(const std::string& newCurrentLayer)
    {
        m_CurrentLayer = newCurrentLayer;
    }

    const std::string& Tilemap::GetCurrentLayer()
    {
        return m_CurrentLayer;
    }

    void Tilemap::AddLayer(const std::string& layerName)
    {
        if (std::find(m_LayerOrder.begin(), m_LayerOrder.end(), layerName) == m_LayerOrder.end())
        {
            m_LayerOrder.push_back(layerName);
            m_Layers.try_emplace(layerName);
        }
        else
        {
            std::cout << "Layer with name '" << layerName << "' already exists in the layer order." << std::endl;
        }
    }

    void Tilemap::AddLayer(const std::string& layerName, const CartesianGrid& grid)
    {
        if (std::find(m_LayerOrder.begin(), m_LayerOrder.end(), layerName) == m_LayerOrder.end())
        {
            m_LayerOrder.push_back(layerName);
            m_Layers.try_emplace(layerName, grid);
        }
        else
        {
            std::cout << "Layer with name '" << layerName << "' already exists in the layer order." << std::endl;
        }
    }

    void Tilemap::RemoveLayer(const std::string& layerName)
    {
        m_LayerOrder.erase(std::remove(m_LayerOrder.begin(), m_LayerOrder.end(), layerName), m_LayerOrder.end());
        m_Layers.erase(layerName);
    }

    void Tilemap::OrderLayer(const std::vector<std::string>& newOrder)
    {
        m_LayerOrder = newOrder;
    }

    void Tilemap::PlaceTile(Point gridPosition, bool resize)
    {
        auto& grid = m_Layers[m_CurrentLayer];

        if (m_SelectedTile.IsValid())
        {
            grid.SetTile(gridPosition, m_SelectedTile.tilesetID, m_SelectedTile.index, m_FlipTileX, m_FlipTileY, m_CurrentRotation, resize);
        }
    }

    void Tilemap::RemoveTile(Point gridPosition)
    {
        auto& grid = m_Layers[m_CurrentLayer];

        if (grid.HasTile(gridPosition))
        {
            grid.RemoveTile(gridPosition);
        }
    }

    void Tilemap::GetTileInfo(Point gridPosition)
    {
        auto& grid = m_Layers[m_CurrentLayer];

        if (grid.HasTile(gridPosition))
        {
            const TileMapTile& tile = grid.GetTile(gridPosition);

            m_SelectedTile = tile;
            m_FlipTileX = tile.flipX;
            m_FlipTileY = tile.flipY;
            m_CurrentRotation = tile.rotation;
        }
    }

    const TileMapTile& Tilemap::GetTile(Point gridPosition, const std::string& layerName)
    {
        auto& grid = m_Layers[layerName];

        return grid.GetTile(gridPosition);
    }

    const TileMapTile& Tilemap::GetTile(Point gridPosition)
    {
        auto& grid = m_Layers[m_CurrentLayer];

        return grid.GetTile(gridPosition);
    }

    void Tilemap::FillTiles(Point gridPosition, const TileMapTile referenceTile)
    {
        auto& grid = m_Layers[m_CurrentLayer];

        if (!grid.IsValidPosition(gridPosition)) return;

        const TileMapTile& currentTile = grid.GetTile(gridPosition);
        bool isReferenceTile = currentTile.tilesetID == referenceTile.tilesetID && currentTile.index == referenceTile.index;
        bool isFilled = currentTile.tilesetID != m_SelectedTile.tilesetID || currentTile.index != m_SelectedTile.index;

        if (isReferenceTile && isFilled)
        {
            grid.SetTile(gridPosition, m_SelectedTile.tilesetID, m_SelectedTile.index, m_SelectedTile.flipX, m_SelectedTile.flipY, m_SelectedTile.rotation, false);

            for (const auto& [neighbour, tilePosition] : grid.GetSurroundingTiles(gridPosition))
            {
                FillTiles(tilePosition, referenceTile);
            }
        }
    }

    bool Tilemap::OnKeyPressed(const KeyPressedEvent& e)
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

        return false;
    }

} // Maize