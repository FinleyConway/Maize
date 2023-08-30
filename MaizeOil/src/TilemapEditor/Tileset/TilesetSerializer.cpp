#include "TilesetSerializer.h"

namespace Maize {

    void TilesetSerializer::SerializeTileset(const Tileset& tileset) const
    {
        nlohmann::json serialTileset;

        serialTileset["ID"] = tileset.GetID();
        serialTileset["Name"] = tileset.GetName();
        serialTileset["CellSize"] = tileset.GetCellSize();
        serialTileset["FilePath"] = tileset.GetPath();

        std::string fileName = tileset.GetName() + ".tileset";
        std::ofstream output(fileName);
        output << std::setw(4) << serialTileset << std::endl;
        output.close();
    }

    std::optional<Tileset> TilesetSerializer::DeserializeTileset(const std::string& filePath) const
    {
        std::ifstream input(filePath);
        if (input.is_open())
        {
            nlohmann::json serialTileset;
            input >> serialTileset;

            int32_t id = serialTileset["ID"];
            std::string name = serialTileset["Name"];
            uint32_t cellsize = serialTileset["CellSize"];
            std::string tilePath = serialTileset["FilePath"];

            input.close();

            return Tileset(id, name, tilePath, cellsize);
        }

        input.close();

        return std::nullopt;
    }

} // Maize