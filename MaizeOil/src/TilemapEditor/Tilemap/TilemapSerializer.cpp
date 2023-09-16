
#include "TilemapSerializer.h"

namespace Maize {

    void TilemapSerializer::SerializeTilemap(std::vector<TilemapLayer> &tilemapLayers)
    {
        nlohmann::json serialTilemapLayers;

        nlohmann::json layerMap;

        for (auto& layer : tilemapLayers)
        {
            nlohmann::json tilemapLayer;

            tilemapLayer["LayerName"] = layer.GetName();

            const CartesianGrid& grid = layer.GetGrid();
            Point gridSize = grid.GetSize();
            int32_t halfWidth = gridSize.x / 2;
            int32_t halfHeight = gridSize.y / 2;

            nlohmann::json tilemap;

            for (int32_t y = -halfHeight; y < halfHeight; y++)
            {
                for (int32_t x = -halfWidth; x < halfWidth; x++)
                {
                    const TilemapTile& tile = layer.GetTile(Point(x, y));

                    if (tile.IsValid())
                    {
                        nlohmann::json serialTile;

                        serialTile["TileIndex"] = tile.index;
                        serialTile["TilesetID"] = tile.tilesetID;
                        serialTile["PositionX"] = x;
                        serialTile["PositionY"] = y;
                        serialTile["FlipX"] = tile.flipX;
                        serialTile["FlipY"] = tile.flipY;
                        serialTile["Rotation"] = tile.rotation;

                        tilemap.push_back(serialTile);
                    }
                }
            }

            tilemapLayer["Tilemap"] = tilemap;

            layerMap.push_back(tilemapLayer);
        }

        serialTilemapLayers["TilemapLayers"] = layerMap;

        std::string fileName = "TestMapName.tilemap";
        std::ofstream output(fileName);
        output << std::setw(4) << serialTilemapLayers << std::endl;
        output.close();
    }

} // Maize