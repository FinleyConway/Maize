#pragma once

#include <Maize.h>

#include "../../CartesianGrid.h"

namespace Maize {

    class Tilemap
    {
    public:
        void OnEvent(Event& e);

        const std::unordered_map<std::string, CartesianGrid>& GetLayers() const { return m_Layers; }
        const std::vector<std::string>& GetLayerOrder() const { return m_LayerOrder; }

        Point GetGridSize(const std::string& layerName) const
        {
            if (auto it = m_Layers.begin(); it != m_Layers.end())
            {
                return m_Layers.at(layerName).GetSize();
            }

            std::cout << "Can't get grid size from layer name: " << layerName << std::endl;

            return Point(0, 0);
        }

        void SetSelectedTile(int32_t tilesetID, int32_t index)
        {
            m_SelectedTile.tilesetID = tilesetID;
            m_SelectedTile.index = index;
        }

        void SetCurrentLayer(const std::string& newCurrentLayer);
        const std::string& GetCurrentLayer();
        void AddLayer(const std::string& layerName);
        void AddLayer(const std::string& layerName, const CartesianGrid& grid);
        void RemoveLayer(const std::string& layerName);
        void OrderLayer(const std::vector<std::string>& newOrder);

        void PlaceTile(Point gridPosition, bool resize = true);
        void RemoveTile(Point gridPosition);
        void GetTileInfo(Point gridPosition);
        const TileMapTile& GetTile(Point gridPosition, const std::string& layerName);
        const TileMapTile& GetTile(Point gridPosition);
        void FillTiles(Point gridPosition, const TileMapTile referenceTile);

        Point ConvertScreenToGrid(PointF mousePosition) const
        {
            return CartesianGrid::ConvertScreenToGrid(mousePosition, m_CellSize);
        }

        PointF ConvertGridToScreen(Point gridPosition) const
        {
            return CartesianGrid::ConvertGridToScreen(gridPosition, m_CellSize);
        }

    private:
        bool OnKeyPressed(const KeyPressedEvent& e);

    private:
        std::unordered_map<std::string, CartesianGrid> m_Layers;
        std::vector<std::string> m_LayerOrder;
        std::string m_CurrentLayer = "Default";
        uint32_t m_CellSize = 8;

        TileMapTile m_SelectedTile;
        bool m_FlipTileX = false;
        bool m_FlipTileY = false;
        float m_CurrentRotation = 0.0f;
    };

} // Maize