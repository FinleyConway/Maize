#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "Tileset.h"

namespace Maize {

    class TilesetSerializer
    {
    public:
        void SerializeTileset(std::vector<Tileset>& tilesets) const;
        std::vector<Tileset> DeserializeTileset(const std::string& filePath) const;
    };

} // Maize