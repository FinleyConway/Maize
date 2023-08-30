#pragma once

#include <fstream>
#include <nlohmann/json.hpp>

#include "../../CartesianGrid.h"

namespace Maize {

    class TilemapSerializer
    {
    public:
        void SerialSizeTilemap(const CartesianGrid& tilemap) const;
        std::optional<CartesianGrid> DeserializeTilemap(const std::string& filePath) const;
    };

} // Maize