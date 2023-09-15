#include "TilesetSerializer.h"

namespace Maize {

    void TilesetSerializer::SerializeTileset(std::vector<Tileset> &tilesets) const
    {
        nlohmann::json serialTilesetArray;

        for (auto& tileset : tilesets)
        {
            nlohmann::json serialTileset;

            serialTileset["ID"] = tileset.GetID();
            serialTileset["Name"] = tileset.GetName();
            serialTileset["FilePath"] = tileset.GetFilePath();

            serialTileset["TileSizeX"] = tileset.GetTileSizeX();
            serialTileset["TileSizeY"] = tileset.GetTileSizeY();

            nlohmann::json tilesetTiles;

            if (tileset.HasTexture())
            {
                nlohmann::json tiles;

                int32_t tilesetSizeX = tileset.GetTexture()->GetWidth() / tileset.GetTileSizeX();
                int32_t tilesetSizeY = tileset.GetTexture()->GetHeight() / tileset.GetTileSizeY();

                for (int32_t x = 0; x < tilesetSizeX; x++)
                {
                    for (int32_t y = 0; y < tilesetSizeY; y++)
                    {
                        int32_t index = x + y * tilesetSizeX;
                        auto* tile = tileset.GetTile(index);

                        if (tile == nullptr) continue;

                        nlohmann::json tileData;
                        tileData["PositionX"] = x;
                        tileData["PositionY"] = y;
                        tileData["IsIncluded"] = tile->IsIncluded();
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

            for (auto& serialTileset : serialTilesetArray)
            {
                Tileset tileset;

                tileset.SetID(serialTileset["ID"]);
                tileset.SetName(serialTileset["Name"]);

                std::string path = serialTileset["FilePath"];
                if (!path.empty())
                    tileset.SetTexture(path);

                tileset.SetTileSizeX(serialTileset["TileSizeX"]);
                tileset.SetTileSizeY(serialTileset["TileSizeY"]);

                tileset.InitEmptyTiles();

                const auto& tilesetTiles = serialTileset["TilesetTiles"];
                for (const auto& tileData : tilesetTiles)
                {
                    int32_t x = tileData["PositionX"];
                    int32_t y = tileData["PositionY"];
                    int32_t numTilesX = tileset.GetTexture()->GetWidth() / tileset.GetTileSizeX();
                    int32_t index = x + y * numTilesX;
                    auto* tile = tileset.GetTile(index);

                    tile->IsIncluded(tileData["IsIncluded"]);
                }

                tilesets.push_back(tileset);
            }

            input.close();
        }

        return tilesets;
    }

} // Maize