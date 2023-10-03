#pragma once

#include "../../../../../Build/Release/_deps/nlohmann_json-src/include/nlohmann/json.hpp"

#include "Maize/Tilemap/Tileset.h"

namespace Maize {

    class TilesetSerializer
    {
    public:
        void SerializeTileset(const std::unordered_map<int32_t, Tileset>& tilesets) const;
        std::unordered_map<int32_t, Tileset> DeserializeTileset(const std::string& filePath) const;
    };

} // Maize