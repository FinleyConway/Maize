#include "mpch.h"
#include "TilemapSerializer.h"

namespace Maize {

    void TilemapSerializer::SerializeTilemap(std::vector<TilemapLayer>& tilemapLayers)
    {
        nlohmann::json serialTilemapLayers;

        for (auto& layer : tilemapLayers)
        {
            nlohmann::json tilemapLayer;
            tilemapLayer["LayerName"] = layer.GetName();

            const CartesianGrid<TilemapTile>& grid = layer.GetGrid();
            sf::Vector2i gridSize = grid.GridSize();
            int32_t halfWidth = gridSize.x / 2;
            int32_t halfHeight = gridSize.y / 2;

            nlohmann::json tilemap;

            for (int32_t y = -halfHeight; y < halfHeight; ++y)
            {
                for (int32_t x = -halfWidth; x < halfWidth; ++x)
                {
                    const TilemapTile& tile = layer.GetTile(sf::Vector2i(x, y));

                    if (tile.IsValid())
                    {
                        /*nlohmann::json serialTile = {
                                {"TexCoordsX", tile.texCoords.x},
                                {"TexCoordsY", tile.texCoords.y},
                                {"PositionX", x},
                                {"PositionY", y},
                                {"FlipX", tile.flipX},
                                {"FlipY", tile.flipY},
                                {"Rotation", tile.rotation}
                        };

                        tilemap.push_back(serialTile);*/
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

                    /*sf::Vector2i position = sf::Vector2i(serialTile["PositionX"], serialTile["PositionY"]);
                    tile.texCoords.x = serialTile["TexCoordsX"];
                    tile.texCoords.y = serialTile["TexCoordsY"];
                    tile.flipX = serialTile["FlipX"];
                    tile.flipY = serialTile["FlipY"];
                    tile.rotation = serialTile["Rotation"];*/

                    //layer.PlaceTile(tile, position, tile.flipX, tile.flipY, tile.rotation);
                }

                tilemapLayers.push_back(layer);
            }
        }
		else
		{
			std::cout << "Could not find or open" << " " << filePath << std::endl;
		}

		input.close();

        return tilemapLayers;
    }

} // Maize