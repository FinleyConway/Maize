#pragma once

#include "../../../../../Build/Release/_deps/nlohmann_json-src/include/nlohmann/json.hpp"

#include "Maize/Tilemap/TilemapLayer.h"

namespace Maize {

    class TilemapSerializer
    {
    public:
        void SerializeTilemap(std::vector<TilemapLayer>& tilemapLayers);
        std::vector<TilemapLayer> DeserializeTilemap(const std::string& filePath);
    };

} // Maize