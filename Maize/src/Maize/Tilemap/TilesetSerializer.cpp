#include "mpch.h"
#include "TilesetSerializer.h"

namespace Maize {

    void TilesetSerializer::SerializeTileset(const std::vector<Tileset> &tilesets) const
    {
        nlohmann::json serialTilesetArray;

        for (const auto& tileset : tilesets)
        {
            nlohmann::json serialTileset;

            serialTileset["ID"] = tileset.GetID();
            serialTileset["Name"] = tileset.GetName();
            serialTileset["FilePath"] = tileset.GetFilePath();

            serialTileset["TileSizeX"] = tileset.GetTileSize().x;
            serialTileset["TileSizeY"] = tileset.GetTileSize().y;

            nlohmann::json tilesetTiles;

            if (tileset.HasTexture())
            {
                nlohmann::json tiles;

                int32_t tilesetSizeX = tileset.GetTexture()->GetWidth() / tileset.GetTileSize().x;
                int32_t tilesetSizeY = tileset.GetTexture()->GetHeight() / tileset.GetTileSize().y;

                for (int32_t x = 0; x < tilesetSizeX; x++)
                {
                    for (int32_t y = 0; y < tilesetSizeY; y++)
                    {
                        int32_t index = x + y * tilesetSizeX;
                        const Tile* tile = tileset.GetTile(index);

                        if (tile == nullptr) continue;

                        nlohmann::json tileData;
                        tileData["PositionX"] = x;
                        tileData["PositionY"] = y;
                        tilesetTiles.push_back(tileData);
                    }
                }
            }

            serialTileset["TilesetTiles"] = tilesetTiles;

            serialTilesetArray.push_back(serialTileset);
        }

        std::string fileName = "tilesets.tilesets";
        std::ofstream output(fileName);
        output << std::setw(4) << serialTilesetArray << std::endl;
        output.close();
    }

    std::vector<Tileset> TilesetSerializer::DeserializeTileset(const std::string &filePath) const
    {
        std::vector<Tileset> tilesets;

        std::ifstream input(filePath);

        if (input.is_open())
        {
            nlohmann::json serialTilesetArray;
            input >> serialTilesetArray;

            for (const auto& serialTileset : serialTilesetArray)
            {
                Tileset tileset;

                tileset.SetID(serialTileset["ID"]);
                tileset.SetName(serialTileset["Name"]);

                std::string path = serialTileset["FilePath"];
                if (!path.empty())
                    tileset.SetTexture(path);

                tileset.SetTileSize(sf::Vector2i(serialTileset["TileSizeX"], serialTileset["TileSizeY"]));

                tileset.InitEmptyTiles();

                const auto& tilesetTiles = serialTileset["TilesetTiles"];
                for (const auto& tileData : tilesetTiles)
                {
                    int32_t x = tileData["PositionX"];
                    int32_t y = tileData["PositionY"];

					tileset.IncludeTile(x, y);
                }

                tilesets.push_back(tileset);
            }

        }
		else
		{
			std::cout << "Could not find or open" << " " << filePath << std::endl;
		}

		input.close();

        return tilesets;
    }

} // Maize