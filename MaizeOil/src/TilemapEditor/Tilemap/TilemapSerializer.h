#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "TilemapLayer.h"

namespace Maize {

    class TilemapSerializer
    {
    public:
        void SerializeTilemap(std::vector<TilemapLayer>& tilemapLayers);
    };

} // Maize