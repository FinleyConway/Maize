#pragma once

#include <nlohmann/json.hpp>

#include "Maize/Tilemap/Tileset.h"

namespace Maize {

    class TilesetSerializer
    {
    public:
        void SerializeTileset(const std::vector<Tileset>& tilesets) const;
        std::vector<Tileset> DeserializeTileset(const std::string& filePath) const;
    };

} // Maize