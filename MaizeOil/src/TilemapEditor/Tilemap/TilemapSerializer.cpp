#include "TilemapSerializer.h"

namespace Maize {

    void TilemapSerializer::SerialSizeTilemap(const CartesianGrid& tilemap) const
    {
        nlohmann::json serialTilemap;

        Point gridSize = tilemap.GetSize();
        int32_t halfWidth = gridSize.x / 2;
        int32_t halfHeight = gridSize.y / 2;

        nlohmann::json tileArray;
        for (int32_t y = -halfHeight; y < halfHeight; y++)
        {
            for (int32_t x = -halfWidth; x < halfWidth; x++)
            {
                const TileMapTile& tilemapTile = tilemap.GetTile(Point(x, y));

                if (tilemapTile.IsValid())
                {
                    nlohmann::json serialTile;

                    serialTile["Tile Index"] = tilemapTile.index;
                    serialTile["Tileset ID"] = tilemapTile.tilesetID;
                    serialTile["Position x"] = x;
                    serialTile["Position y"] = y;
                    serialTile["Flip x"] = tilemapTile.flipX;
                    serialTile["Flip y"] = tilemapTile.flipY;
                    serialTile["Rotation"] = tilemapTile.rotation;

                    tileArray.push_back(serialTile);
                }
            }
        }

        serialTilemap["Tiles"] = tileArray;

        std::string fileName = "TestMapName.tilemap";
        std::ofstream output(fileName);
        output << std::setw(4) << serialTilemap << std::endl;
        output.close();
    }

    std::optional<CartesianGrid> TilemapSerializer::DeserializeTilemap(const std::string& filePath) const
    {
        CartesianGrid grid;

        std::ifstream input(filePath);
        if (input.is_open())
        {
            nlohmann::json serialTilemap;
            input >> serialTilemap;

            for (const auto& serialTile : serialTilemap["Tiles"])
            {
                int32_t index = serialTile["Tile Index"];
                int32_t tilesetID = serialTile["Tileset ID"];
                Point position = Point(serialTile["Position x"], serialTile["Position y"]);
                bool flipX = serialTile["Flip x"];
                bool flipY = serialTile["Flip y"];
                float rotation = serialTile["Rotation"];

                grid.SetTile(position, tilesetID, index, flipX, flipY, rotation);
            }

            input.close();
            input.close();

            return grid;
        }

        input.close();

        return std::nullopt;
    }

} // Maize