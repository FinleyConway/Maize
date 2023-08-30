#pragma once

#include "Tileset.h"
#include "TilesetSerializer.h"

namespace Maize {

    class TilesetManager
    {
    public:
        std::unordered_map<int32_t, Tileset>& GetTilesets() { return m_Tilesets; }

        void SetSelectedTileset(Tileset* tileset) { m_SelectedTileset = tileset; }
        Tileset* GetSelectedTileset() { return m_SelectedTileset; }

        Tileset& CreateTileset(const std::string& name, const std::string& filepath, uint32_t cellSize)
        {
            int32_t id = m_Tilesets.size(); // temp way of getting an id

            Tileset tileset = Tileset(id, name, filepath, cellSize);

            m_Tilesets.try_emplace(id, tileset);

            return m_Tilesets.at(id);
        }

        void SaveTileset(int32_t tilesetID)
        {
            if (auto it = m_Tilesets.find(tilesetID); it != m_Tilesets.end())
            {
                m_Serializer.SerializeTileset(m_Tilesets.at(tilesetID));
            }

            std::cout << "Trying to save an invalid tileset ID: " << tilesetID << std::endl;
        }

        void LoadTileset(const std::string& filepath)
        {
            auto tileset = m_Serializer.DeserializeTileset(filepath);
            if (tileset.has_value())
            {
                auto& value = tileset.value();
                m_Tilesets.try_emplace(value.GetID(), value);
            }
        }

    private:
        std::unordered_map<int32_t, Tileset> m_Tilesets;
        Tileset* m_SelectedTileset = nullptr;
        TilesetSerializer m_Serializer;
    };

} // Maize