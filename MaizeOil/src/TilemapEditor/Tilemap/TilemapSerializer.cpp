
#include "TilemapSerializer.h"

namespace Maize {

    void TilemapSerializer::SerializeTilemap(std::vector<TilemapLayer>& tilemapLayers)
    {
        nlohmann::json serialTilemapLayers;

        for (auto& layer : tilemapLayers)
        {
            nlohmann::json tilemapLayer;
            tilemapLayer["LayerName"] = layer.GetName();

            const CartesianGrid& grid = layer.GetGrid();
            Point gridSize = grid.GetSize();
            int32_t halfWidth = gridSize.x / 2;
            int32_t halfHeight = gridSize.y / 2;

            nlohmann::json tilemap;

            for (int32_t y = -halfHeight; y < halfHeight; ++y)
            {
                for (int32_t x = -halfWidth; x < halfWidth; ++x)
                {
                    const TilemapTile& tile = layer.GetTile(Point(x, y));

                    if (tile.IsValid())
                    {
                        nlohmann::json serialTile = {
                                {"TileIndex", tile.index},
                                {"TilesetID", tile.tilesetID},
                                {"PositionX", x},
                                {"PositionY", y},
                                {"FlipX", tile.flipX},
                                {"FlipY", tile.flipY},
                                {"Rotation", tile.rotation}
                        };

                        tilemap.push_back(serialTile);
                    }
                }
            }

            tilemapLayer["Tilemap"] = tilemap;
            serialTilemapLayers["TilemapLayers"].push_back(tilemapLayer);
        }

        std::string fileName = "TestMapName.tilemap";
        std::ofstream output(fileName);
        output << std::setw(4) << serialTilemapLayers << std::endl;
        output.close();
    }

    std::vector<TilemapLayer> TilemapSerializer::DeserializeTilemap(const std::string &filePath)
    {
        std::vector<TilemapLayer> tilemapLayers;

        std::ifstream input(filePath);
        if (input.is_open())
        {
            nlohmann::json serialTilemapLayers;
            input >> serialTilemapLayers;

            for (auto& tilemapLayer : serialTilemapLayers["TilemapLayers"])
            {
                TilemapLayer layer;
                layer.SetName(tilemapLayer["LayerName"]);

                for (auto& serialTile : tilemapLayer["Tilemap"])
                {
                    TilemapTile tile;

                    Point position = Point(serialTile["PositionX"], serialTile["PositionY"]);
                    tile.index = serialTile["TileIndex"];
                    tile.tilesetID = serialTile["TilesetID"];
                    tile.flipX = serialTile["FlipX"];
                    tile.flipY = serialTile["FlipY"];
                    tile.rotation = serialTile["Rotation"];

                    layer.PlaceTile(tile, position, tile.flipX, tile.flipY, tile.rotation);
                }

                tilemapLayers.push_back(layer);
            }
        }

        return tilemapLayers;
    }

} // Maize