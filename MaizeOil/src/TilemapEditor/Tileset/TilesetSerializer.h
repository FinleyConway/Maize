#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "Tileset.h"

namespace Maize {

    class TilesetSerializer
    {
    public:
        void SerializeTileset(const Tileset& tileset) const;
        std::optional<Tileset> DeserializeTileset(const std::string& filePath) const;
    };

} // Maize